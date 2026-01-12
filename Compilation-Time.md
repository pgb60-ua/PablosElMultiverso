# Análisis de Rendimiento en la Compilación

## 1. Compilación con diferentes números de trabajos (`make -jN`)

Se midieron los tiempos de compilación para distintos valores de **N** (número de trabajos en paralelo):

| N (hilos) | real | user | sys |
|------------|-------|-------|------|
| 1 | 1m21.547s | 0m17.023s | 0m5.233s |
| 2 | 0m32.305s | 0m20.781s | 0m4.726s |
| 3 | 0m22.824s | 0m22.146s | 0m4.641s |
| 4 | 0m21.105s | 0m27.442s | 0m5.486s |
| 5 | 0m17.644s | 0m31.368s | 0m6.373s |
| 6 | 0m15.949s | 0m34.052s | 0m6.569s |
| 7 | 0m15.044s | 0m37.372s | 0m7.104s |
| 8 | 0m13.588s | 0m38.092s | 0m7.928s |

### Análisis de resultados

#### 1. ¿A partir de qué valores de N ya no supone una mejora sustancial?

| N (hilos) | Tiempo real | Mejora vs anterior | Mejora acumulada |
|-----------|-------------|-------------------|------------------|
| 1 → 2 | 1m21s → 32s | **-60.3%** | -60.3% |
| 2 → 3 | 32s → 23s | **-29.4%** | -72.0% |
| 3 → 4 | 23s → 21s | **-7.6%** | -74.1% |
| 4 → 5 | 21s → 18s | **-16.4%** | -78.4% |
| 5 → 6 | 18s → 16s | **-9.6%** | -80.4% |
| 6 → 7 | 16s → 15s | **-5.7%** | -81.5% |
| 7 → 8 | 15s → 14s | **-9.7%** | -83.3% |

**Respuesta:** 

Las mejoras más significativas ocurren en los primeros valores:
- **N=1→2:** Reducción del 60.3% (mejora más dramática por primera paralelización)
- **N=2→3:** Reducción del 29.4% (aún muy significativa)
- **N=3→5:** Mejoras del 7-16% (todavía sustanciales)

A partir de **N=6**, la mejora se estabiliza por debajo del 10% entre valores consecutivos:
- **N=6→7:** Solo 5.7% de mejora
- **N=7→8:** 9.7% de mejora

**Conclusión:** El límite de mejora sustancial está en **N=5-6**. A partir de **N=6**, los incrementos son marginales, indicando que se ha alcanzado el límite práctico de paralelización del sistema. 

El valor óptimo para este sistema es **N=8**, aunque N=6 ofrece un equilibrio excelente entre rendimiento y uso de recursos.

> **Nota:** Para aprovechar automáticamente el máximo número de núcleos disponibles, se puede usar el comando `nproc` en el Makefile para configurar dinámicamente `--jobs=$(nproc)`.

---

## 2. Fallos de compilación en paralelo

#### 2. ¿Hay fallos de compilación con ejecuciones en paralelo?

**Respuesta:** Durante las pruebas con `make -jN`, **no se detectaron fallos de compilación**. Las dependencias del Makefile están correctamente definidas, por lo que **la compilación paralela fue estable**.

Es importante destacar que _make all_ es completamente estable en ejecución paralela. Sin embargo, la funcionalidad _make rebuild_ (que ejecuta _make clean_ seguido de _make all_) originalmente presentaba fallos intermitentes, ya que se paralelizaba la ejecución del clean y el all simultáneamente, provocando condiciones de carrera. Este problema se solucionó cambiando a llamadas recursivas secuenciales en los pasos del _make_, garantizando que _clean_ finalice antes de iniciar _all_.


---

## 3. Compilación con `ccache`

#### 3. Comparación de tiempo con y sin ccache

| Situación | Tiempo real |
|-----------|-------------|
| **Sin ccache** (make -j8) | **13.6s** |
| **Con ccache** (1ª compilación, caché vacía) | **13.6s** |
| **Con ccache** (2ª compilación, caché llena) | **1.7s** |

**Mejora:** De 13.6s → 1.7s = **700% más rápido** (8x más rápido)

**Respuesta:** `ccache` no mejora la primera compilación, pero **reduce el tiempo en un 87.5% en compilaciones subsiguientes** cuando solo hay cambios incrementales, ya que evita recompilar los archivos sin cambios, reutilizando los resultados almacenados.

---

## 4. Limpieza y análisis de estadísticas de `ccache`

#### 4. Estadísticas de ccache tras dos compilaciones

Para analizar el rendimiento de `ccache`, primero se limpian las estadísticas y la caché:

```bash
ccache --zero-stats  
ccache --clear      
```
o bien usamos
```bash
make clean-cache
```

Tras compilar dos veces consecutivas, se consultan las estadísticas con:

```bash
ccache --show-stats
```

**Resultados de la primera compilación (sin caché):**
```
Cacheable calls:     23 /  24 (95.83%)
  Hits:               0 /  23 ( 0.00%)
    Direct:           0
    Preprocessed:     0
  Misses:            23 /  23 (100.0%)
Uncacheable calls:    1 /  24 ( 4.17%)
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.01%)
  Hits:               0 /  23 ( 0.00%)
  Misses:            23 /  23 (100.0%)
```

**Resultados de la segunda compilación (con caché):**
```
Cacheable calls:     46 /  48 (95.83%)
  Hits:              23 /  46 (50.00%)
    Direct:          23 /  23 (100.0%)
    Preprocessed:     0 /  23 ( 0.00%)
  Misses:            23 /  46 (50.00%)
Uncacheable calls:    2 /  48 ( 4.17%)
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.01%)
  Hits:              23 /  46 (50.00%)
  Misses:            23 /  46 (50.00%)
```

**Análisis:**
- **Primera compilación:** 23 archivos compilados desde cero (100% cache miss) y almacenados en caché
- **Segunda compilación:** 23 archivos recuperados de caché directamente (100% hits directos)
- **Hit rate del 50%:** Tasa calculada sobre el total de llamadas acumuladas (23 hits / 46 llamadas totales)
- La segunda compilación reutiliza completamente los resultados cacheados, evitando recompilación

**Respuesta:** `ccache` es altamente efectivo. El 100% de los archivos compilables se reutilizan de caché en la segunda ejecución, lo que explica la drástica reducción de tiempo de 13.6s a 1.7s.

