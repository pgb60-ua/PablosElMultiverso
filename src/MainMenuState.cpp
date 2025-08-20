#include <MainMenuState.hpp>
#include <iostream>

MainMenuState::MainMenuState()
{
}

void MainMenuState::init()
{
    std::cout << "You are in the Main Menu State" << std::endl;
}

void MainMenuState::handleInput()
{
    std::cout << "Enter a key: ";
    this->entered_key = std::cin.get();
}

void MainMenuState::update(float deltaTime)
{
    this->handleInput();
}

void MainMenuState::render()
{
    std::cout << "You entered: " << this->entered_key << std::endl;
}