#pragma once

#include <libintl.h>
#include <locale.h>
#include <string>

#define _(String) gettext(String)

#define N_(String) String

// Función para detectar la ruta de los archivos de traducción
std::string GetLocalePath();

// Función multiplataforma para establecer variables de entorno
void SetEnvironmentVariable(const std::string& name, const std::string& value);