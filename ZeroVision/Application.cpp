#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SceneManager.h"
#include "Menu.h"

#include "GlobalVariables.h"
#include "BeatMapEditor.h"
#include "GamePart.h"
#include "ChainedScenes.h"
#include "HowToPlay.h"
#include "Exit.h"  
#include "Beginning.h"  

Menu* menu;
void toMenu() {
    menu = new Menu;
    if (GlobalVariables::load() != 0) {
        menu->AddMenuOption<ChainedScenes>("Continue", "continue");
    }
    menu->AddMenuOption<ChainedScenes>("New Game", "new_game");
    menu->AddMenuOption<HowToPlay>("How to play", "how_to_play");
    menu->AddMenuOption<Exit>("Exit", "exit");
    //menu->AddMenuOption<BeatMapEditor>("Edit      ", "exit");

    GlobalVariables::scene_manager->attachScene(menu);
    GlobalVariables::menu_request = false;
}

int main()
{   

    GlobalVariables::window->setFramerateLimit(60);
    //beatmap.deserialize("res\\sound\\test\\test.bin");
    GlobalVariables::scene_manager->attachScene(new Beginning); 

    GlobalVariables::beatmap = new BeatMap;

    sf::View view = GlobalVariables::window->getDefaultView();
    while (GlobalVariables::window->isOpen())
    {
        while (const std::optional event = GlobalVariables::window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                GlobalVariables::window->close();
            }else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                view.setSize({
                                        static_cast<float>(resized->size.x),
                                        static_cast<float>(resized->size.y)
                    });
                view.setCenter({ static_cast<float>(resized->size.x/2),static_cast<float>(resized->size.y/2) });
                GlobalVariables::window->setView(view);
            }
        }
        if (GlobalVariables::menu_request) {
            toMenu();
        }

        GlobalVariables::scene_manager->update();
    }
}