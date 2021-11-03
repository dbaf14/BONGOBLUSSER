#include "bn_core.h"
#include "bn_math.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_ptr.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_regular_bg_ptr.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_item.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_item.h"
#include "bn_affine_bg_tiles_ptr.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_camera_actions.h"
#include "bn_sprite_animate_actions.h"

#include "bn_string_view.h"
#include "bn_vector.h"
#include "bn_log.h"
#include "bn_sprite_text_generator.h"

//SCENES
#include "ds_scene_splash.h"
#include "ds_scene_butano.h"
#include "ds_scene_title.h"
#include "ds_scene_file.h"
#include "ds_scene.h"

#include "ds_scene_buttontest.h"
#include "ds_scene_game.h"



int main()
{
    bn::core::init();
    ds::Scene scene = ds::Scene::SPLASH;

     while(true)
    {
        if(scene == ds::Scene::SPLASH)
        {
            ds::Splash splash = ds::Splash();
            scene = splash.execute();
        }
        else if(scene == ds::Scene::TITLE)
        {
            ds::Title title = ds::Title();
            scene = title.execute();
        }
        else if(scene == ds::Scene::BUTANO)
        {
            ds::Butano butano = ds::Butano();
            scene = butano.execute();
        }
        else if(scene == ds::Scene::FILE)
        {
            ds::File file = ds::File();
            scene = file.execute();
        }

	else if(scene == ds::Scene::BUTTONTEST)
        {
            ds::Buttontest buttontest = ds::Buttontest();
            scene = buttontest.execute();
        }

    else if(scene == ds::Scene::GAME)
        {
            ds::Game game = ds::Game();
            scene = game.execute();
        }
        
    
        bn::core::update();  
    } 
}