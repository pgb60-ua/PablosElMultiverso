```mermaid
classDiagram

%% ============================================
%% JERARQUÍA DE ENTIDADES
%% ============================================
AEntity <|-- Player
AEntity <|-- AEnemy
AEnemy <|-- Zombie
AEnemy <|-- Darkin
AEnemy <|-- ChemicalDestructor

%% ============================================
%% JERARQUÍA DE ITEMS Y ARMAS
%% ============================================
Item <|-- AWeapon
Item <|-- ConcreteItem
AWeapon <|-- AMeleeWeapon
AWeapon <|-- ARangeWeapon
AMeleeWeapon <|-- AxeWeapon
AMeleeWeapon <|-- SwordWeapon
AMeleeWeapon <|-- ScytheWeapon
ARangeWeapon <|-- WandWeapon
ARangeWeapon <|-- EggplosiveWeapon
ARangeWeapon <|-- LaserRayWeapon
ARangeWeapon <|-- SniperWeapon
ARangeWeapon <|-- WingWeapon
ARangeWeapon <|-- ChemicalDestructorWeapon

%% ============================================
%% JERARQUÍA DE PROYECTILES
%% ============================================
AProjectile <|-- WandProjectile
AProjectile <|-- EggplosiveProjectile
AProjectile <|-- LaserRayProjectile
AProjectile <|-- SniperProjectile
AProjectile <|-- WingProjectile
AProjectile <|-- ChemicalDestructorProjectile

%% ============================================
%% JERARQUÍA DE ESTADOS (STATE PATTERN)
%% ============================================
GameState <|-- MainMenuState
GameState <|-- ChooseNPCMenuState
GameState <|-- MainGameState
GameState <|-- ShopState
GameState <|-- GameOverState
GameState <|-- GameWonState

%% ============================================
%% RELACIONES DE COMPOSICIÓN (ARMAS-PROYECTILES)
%% ============================================
WandWeapon "1" o-- "0..*" WandProjectile : contains
EggplosiveWeapon "1" o-- "0..*" EggplosiveProjectile : contains
LaserRayWeapon "1" o-- "0..*" LaserRayProjectile : contains
SniperWeapon "1" o-- "0..*" SniperProjectile : contains
WingWeapon "1" o-- "0..*" WingProjectile : contains
ChemicalDestructorWeapon "1" o-- "0..*" ChemicalDestructorProjectile : contains

%% ============================================
%% RELACIONES CON STATS
%% ============================================
Stats *-- OffensiveStats : contains
Stats *-- DefensiveStats : contains
AEntity *-- Stats : has
Item *-- Stats : has
AProjectile *-- Stats : has

%% ============================================
%% RELACIONES CON GEOMETRÍA
%% ============================================
AEntity *-- Shape : has
AProjectile *-- Shape : has
Shape *-- ShapeData : contains
Shape -- ShapeType : uses

%% ============================================
%% RELACIONES CON SPRITES
%% ============================================
AEntity *-- SpriteAnimation : has
AWeapon *-- SpriteAnimation : has
AProjectile *-- SpriteAnimation : has
SpriteLoaderManager ..> SpriteSheet : creates
CharacterOption *-- SpriteAnimation : has

%% ============================================
%% RELACIONES PLAYER
%% ============================================
Player "1" o-- "0..*" Item : inventory
Player "1" o-- "0..4" AWeapon : weapons
Player "1" --> "0..*" AEnemy : targets
Player -- PLAYER_TYPE : uses

%% ============================================
%% RELACIONES ENEMY
%% ============================================
AEnemy "0..*" --> "1..*" Player : targets
AEnemy -- ENEMY_TYPE : uses
AEnemy ..> AEnemy : boids algorithm

%% ============================================
%% RELACIONES WEAPON
%% ============================================
AWeapon "1" --> "0..*" AEnemy : targets
AWeapon -- WEAPON_TYPE : uses
AWeapon -- ItemRarity : uses

%% ============================================
%% RELACIONES PROJECTILE
%% ============================================
AProjectile "1" --> "0..*" AEnemy : targets
ChemicalDestructorProjectile "1" --> "0..*" Player : targets
AProjectile -- PROJECTILE_TYPE : uses

%% ============================================
%% RELACIONES ITEM
%% ============================================
Item -- ITEM_TYPE : uses
Item -- ItemRarity : uses

%% ============================================
%% FACTORIES Y MANAGERS
%% ============================================
WeaponFactory ..> AWeapon : creates
WeaponFactory -- ITEM_TYPE : uses
WeaponFactory -- WEAPON_TYPE : uses
WeaponFactory -- PLAYER_TYPE : uses
ItemsFactory ..> Item : creates
ItemsFactory ..> DataFileManager : uses

%% ============================================
%% SHOP
%% ============================================
Shop "1" o-- "5" TShopSlot : contains
Shop ..> ItemsFactory : uses
Shop ..> Player : interacts
TShopSlot --> Item : references
ShopState ..> Shop : contains
ShopState ..> ShopLayout : uses
ShopState ..> RenderableItem : uses
ShopState ..> TWeaponColor : uses

%% ============================================
%% STATE MACHINE
%% ============================================
StateMachine "1" o-- "1..*" GameState : manages
GameState --> StateMachine : transitions

%% ============================================
%% MAIN GAME STATE Y ROUNDS
%% ============================================
MainGameState "1" o-- "1..*" Player : contains
MainGameState "1" o-- "0..*" AEnemy : contains
MainGameState "1" *-- "1" RoundManager : contains
RoundManager "1" *-- "1" Round : current
RoundManager "1" o-- "1..*" RoundInfo : contains
Round "1" --> "0..*" AEnemy : spawns
RoundManager -- ROUND_TYPE : uses

%% ============================================
%% MANAGERS SINGLETON
%% ============================================
SpriteLoaderManager ..> PLAYER_TYPE : uses
SpriteLoaderManager ..> ENEMY_TYPE : uses
SpriteLoaderManager ..> WEAPON_TYPE : uses
SpriteLoaderManager ..> PROJECTILE_TYPE : uses
SpriteLoaderManager ..> ITEM_TYPE : uses
SpriteLoaderManager ..> MAP_TYPE : uses
SpriteLoaderManager ..> Shape : provides

DataFileManager ..> PLAYER_TYPE : uses
DataFileManager ..> ENEMY_TYPE : uses
DataFileManager ..> WEAPON_TYPE : uses
DataFileManager ..> ITEM_TYPE : uses
DataFileManager ..> ROUND_TYPE : uses
DataFileManager ..> Stats : provides
DataFileManager ..> ItemData : provides
DataFileManager ..> RoundInfo : provides

%% ============================================
%% CHOOSE NPC MENU
%% ============================================
ChooseNPCMenuState ..> CharacterOption : uses
CharacterOption -- PLAYER_TYPE : uses

%% ============================================
%% CLASES Y ESTRUCTURAS
%% ============================================

class AEntity{
    <<abstract>>
    #Stats stats
    #Shape shape
    #SpriteAnimation spriteAnimation
    #Vector2 position
    +Update(float deltaTime)*
    +Render()*
    +CheckCollisions()*
    +GetStats() Stats&
    +GetShape() Shape&
}

class Player{
    -PLAYER_TYPE playerType
    -vector~AEnemy*~ enemiesInRange
    -vector~AEnemy*~ allEnemies
    -vector~const Item*~ inventory
    -vector~unique_ptr~AWeapon~~ weapons
    -float healthModifier
    -float attackSpeedModifier
    -float criticalChanceModifier
    -float criticalDamageModifier
    -float lifeStealModifier
    -float movementSpeedModifier
    -float agilityModifier
    -float armorModifier
    -float resistanceModifier
    -float healthRegenerationModifier
    -float physicalDamageModifier
    -float magicDamageModifier
    -int pabloCoins
    +AddWeapon(ITEM_TYPE)
    +AddItem(const Item*)
    +UpgradeWeapon(int index)
    +SellWeapon(int index)
}

class AEnemy{
    <<abstract>>
    #ENEMY_TYPE enemyType
    #vector~Player*~ objectives
    #float perceptionRadius
    #float separationRadius
    #float alignmentWeight
    #float cohesionWeight
    #float separationWeight
    #static vector~AEnemy*~ s_allEnemies
    +DropLoot()
    #CalculateBoidForce() Vector2
    #CalculateTargetForce() Vector2*
}

class Zombie{
    <<ENEMY_TYPE::ZOMBIE>>
}

class Darkin{
    <<ENEMY_TYPE::DARKIN>>
}

class ChemicalDestructor{
    <<ENEMY_TYPE::CHEMICAL_DESTRUCTOR>>
}

class Item{
    #string name
    #string description
    #Stats stats
    #ItemRarity rarity
    #int price
    #ITEM_TYPE itemType
    +GetName() string
    +GetDescription() string
    +GetStats() Stats
    +GetRarity() ItemRarity
}

class ConcreteItem{

}

class AWeapon{
    <<abstract>>
    #SpriteAnimation spriteAnimation
    #WEAPON_TYPE weaponType
    #int level
    #Vector2 position
    #Vector2 direction
    #vector~AEnemy*~ enemiesInRange
    #vector~AEnemy*~ allEnemies
    +MAXLEVEL = 4
    +Upgrade()
    +Update(float, Vector2, Vector2)*
    +Render()*
}

class AMeleeWeapon{
    <<abstract>>
    #float attackTimer
    #float attackInterval
    #bool attacking
}

class ARangeWeapon{
    <<abstract>>
    #vector~unique_ptr~AProjectile~~ projectilePool
    #float shootTimer
    #void Shoot()*
    #AProjectile* GetInactiveProjectile()
}

class AxeWeapon{
    <<Warrior>>
    -float swingAngle
    -float swingSpeed
}

class SwordWeapon{
    <<Warrior>>
    -float thrustDistance
    -float thrustTimer
}

class ScytheWeapon{
    <<Healer>>
}

class WandWeapon{
    <<Healer>>
}

class EggplosiveWeapon{
    <<Mage>>
}

class LaserRayWeapon{
    <<Mage>>
}

class SniperWeapon{
    <<Range>>
}

class WingWeapon{
    <<Range>>
}

class ChemicalDestructorWeapon{
    <<Enemy Weapon>>
}

class AProjectile{
    <<abstract>>
    #Stats stats
    #Shape shape
    #SpriteAnimation spriteAnimation
    #Vector2 direction
    #vector~AEnemy*~ enemiesInScene
    #bool active
    +Activate(Vector2, Vector2)
    +Deactivate()
    +IsActive() bool
    +Update(float)*
    +Render()*
}

class WandProjectile{
    -float lifetime
}

class EggplosiveProjectile{
    -float explosionRadius
}

class LaserRayProjectile{
    -float duration
}

class SniperProjectile{
    -float speed
}

class WingProjectile{
    -bool returning
}

class ChemicalDestructorProjectile{
    -vector~Player*~ players
}

class WeaponFactory{
    <<static>>
    +CreateWeapon(ITEM_TYPE, vector~AEnemy*~, vector~AEnemy*~) unique_ptr~AWeapon~
    +CreateStartingWeapon(PLAYER_TYPE, vector~AEnemy*~, vector~AEnemy*~) unique_ptr~AWeapon~
}

class ItemsFactory{
    <<singleton>>
    -vector~unique_ptr~Item~~ allItems
    +GetInstance() ItemsFactory&
    +LoadAllItems()
    +GetRandomItems(int) vector~const Item*~
}

class Shop{
    -array~TShopSlot, 5~ shopSlots
    -int rerollCost
    +Reroll()
    +BuyItem(int, Player*)
    +BuyWeapon(int, Player*)
}

class SpriteLoaderManager{
    <<singleton>>
    -unordered_map~PLAYER_TYPE, SpriteSheet~ playerSprites
    -unordered_map~ENEMY_TYPE, SpriteSheet~ enemySprites
    -unordered_map~WEAPON_TYPE, SpriteSheet~ weaponSprites
    -unordered_map~PROJECTILE_TYPE, SpriteSheet~ projectileSprites
    -unordered_map~ITEM_TYPE, SpriteSheet~ itemSprites
    -unordered_map~MAP_TYPE, SpriteSheet~ mapSprites
    +GetInstance() SpriteLoaderManager&
    +GetSpriteSheet(PLAYER_TYPE) SpriteSheet&
    +GetSpriteSheet(ENEMY_TYPE) SpriteSheet&
    +GetSpriteSheet(WEAPON_TYPE) SpriteSheet&
    +GetSpriteSheet(PROJECTILE_TYPE) SpriteSheet&
    +GetSpriteSheet(ITEM_TYPE) SpriteSheet&
    +GetSpriteSheet(MAP_TYPE) SpriteSheet&
    +GetSpriteHitbox(PLAYER_TYPE) Shape
    +GetSpriteHitbox(ENEMY_TYPE) Shape
    +GetSpriteHitbox(WEAPON_TYPE) Shape
}

class DataFileManager{
    <<singleton>>
    +GetInstance() DataFileManager&
    +GetPlayerStats(PLAYER_TYPE) Stats
    +GetEnemyStats(ENEMY_TYPE) Stats
    +GetWeaponStats(WEAPON_TYPE) Stats
    +GetItemStats(ITEM_TYPE) Stats
    +GetItemData(ITEM_TYPE) ItemData
    +GetRounds(ROUND_TYPE) vector~RoundInfo~
}

class GameState{
    <<abstract>>
    #StateMachine* stateMachine
    +Init()*
    +HandleInput()*
    +Update(float)*
    +Render()*
    +Pause()*
    +Resume()*
}

class StateMachine{
    -stack~unique_ptr~GameState~~ states
    +PushState(unique_ptr~GameState~)
    +PopState()
    +ChangeState(unique_ptr~GameState~)
    +Update(float)
    +Render()
    +HandleInput()
}

class MainMenuState{

}

class ChooseNPCMenuState{
    -vector~CharacterOption~ characters
}

class MainGameState{
    -vector~unique_ptr~Player~~ players
    -vector~Player*~ playerPointers
    -vector~AEnemy*~ enemies
    -RoundManager roundManager
    -MAP_TYPE currentMap
}

class ShopState{
    -Shop shop
    -vector~Player*~ players
    -ShopLayout layout
}

class GameOverState{

}

class GameWonState{

}

class RoundManager{
    -vector~RoundInfo~ roundInfos
    -Round currentRound
    -int currentRoundIndex
    -vector~AEnemy*~ enemiesOnMap
    -vector~Player*~ players
    +StartNextRound()
    +IsRoundFinished() bool
    +AreAllRoundsFinished() bool
}

class Round{
    -vector~AEnemy*~ enemiesToSpawn
    -vector~AEnemy*~ enemiesOnMap
    -float duration
    -float spawnRate
    -float timeElapsed
    +Update(float)
    +SpawnEnemy()
}

class Stats{
    +OffensiveStats offensive
    +DefensiveStats defensive
    +operator+(Stats) Stats
    +operator+=(Stats)
}

class OffensiveStats{
    <<struct>>
    +float physicalDamage
    +float magicDamage
    +float attackSpeed
    +float criticalChance
    +float criticalDamage
    +float lifeSteal
}

class DefensiveStats{
    <<struct>>
    +float health
    +float healthMax
    +float movementSpeed
    +float agility
    +float armor
    +float resistance
    +float healthRegeneration
}

class Shape{
    <<struct>>
    +ShapeType type
    +ShapeData data
}

class ShapeData{
    <<union>>
    +Rectangle rectangle
    +RotatedRectangle rotatedRectangle
    +Circle circle
}

class Circle{
    <<struct>>
    +Vector2 center
    +float radius
}

class RotatedRectangle{
    <<struct>>
    +float x
    +float y
    +float width
    +float height
    +float rotation
}

class SpriteSheet{
    <<struct>>
    +Texture2D texture
    +vector~Rectangle~ frames
    +int spriteFrameCount
}

class FrameAnimation{
    <<struct>>
    +int frame
    +float timer
    +float frameDuration
    +bool loop
    +bool finished
}

class SpriteAnimation{
    <<struct>>
    +static const float FRAME_DURATION
    +int frameIndex
    +float timeAccumulator
    +bool flipped
    +Color color
}

class RoundInfo{
    <<struct>>
    +int roundNumber
    +float duration
    +float spawnRate
    +unordered_map~ENEMY_TYPE, int~ enemiesToSpawnCount
}

class TShopSlot{
    <<struct>>
    +const Item* item
    +bool isBlocked
    +bool isBuyed
    +int weaponLevel
}

class ItemData{
    <<struct>>
    +string name
    +string description
    +Stats stats
    +ItemRarity rarity
    +int price
}

class CharacterOption{
    <<struct>>
    +PLAYER_TYPE type
    +string name
    +string description
    +SpriteAnimation spriteAnimation
}

class ShopLayout{
    <<struct>>
    +int screenWidth
    +int screenHeight
    +int headerHeight
    +int statsX
    +int statsY
    +int statsWidth
    +int statsHeight
    +int itemsX
    +int itemsY
    +int itemsWidth
    +int itemsHeight
    +int itemSlotHeight
    +int itemSlotSpacing
    +int itemStartY
    +int weaponSlotSize
    +int weaponSlotSpacing
    +int weaponsPerRow
    +int statSpacing
    +int weaponPanelY
    +int weaponsY
    +int weaponStartX
    +int weaponStartY
}

class RenderableItem{
    <<struct>>
    +ITEM_TYPE itemType
    +string name
    +int level
    +bool isWeapon
    +bool isSelected
    +bool isBlocked
    +int price
    +const Stats* stats
    +string description
    +ItemRarity rarity
}

class TWeaponColor{
    <<struct>>
    +Color borderColor
    +Color tintColor
}

class WeaponData{
    <<struct>>
    +string name
    +string description
    +Stats stats
    +ItemRarity rarity
    +int level
}

class PLAYER_TYPE{
    <<enum>>
    WARRIOR
    MAGE
    RANGE
    HEALER
}

class ENEMY_TYPE{
    <<enum>>
    ZOMBIE
    DARKIN
    CHEMICAL_DESTRUCTOR
}

class ITEM_TYPE{
    <<enum>>
    FIREBALL
    SPINED_BREASTPLATE
    PEARL_EGG
    RAVENS_FEATHER
    VOID_RING
    SATCHEL
    SPIRIT_MASK
    CELESTIAL_SPARK
    EXPLOSIVE_ARSENAL
    MAGICAL_MIRROR
    ETHEREAL_DAGGER
    SPOTLIGHT
    CRIMSON_VIAL
    RUBY_HEART
    RAW_MEAT
    VENOMOUS_PLANT
    COIN
    WEAPON_AXE
    WEAPON_SWORD
    WEAPON_SCYTHE
    WEAPON_WAND
    WEAPON_EGGPLOSIVE
    WEAPON_LASER_RAY
    WEAPON_SNIPER
    WEAPON_WING
}

class WEAPON_TYPE{
    <<enum>>
    AXE
    SWORD
    SCYTHE
    WAND
    EGGPLOSIVE
    LASER_RAY
    SNIPER
    WING
    CHEMICAL_DESTRUCTOR
}

class PROJECTILE_TYPE{
    <<enum>>
    WAND
    EGGPLOSIVE_CIRCLE
    EGGPLOSIVE_BULLET
    LASER_RAY
    SNIPER
    WING
    CHEMICAL_BULLET
}

class MAP_TYPE{
    <<enum>>
    DEFAULT
    DEFAULT_UPPER
}

class ROUND_TYPE{
    <<enum>>
    EASY
    MEDIUM
    HARD
}

class ItemRarity{
    <<enum>>
    Common
    Uncommon
    Rare
    Epic
    Legendary
}

class ShapeType{
    <<enum>>
    SHAPE_RECTANGLE
    SHAPE_ROTATED_RECTANGLE
    SHAPE_CIRCLE
}

```
