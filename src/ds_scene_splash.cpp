#include "ds_scene_splash.h"

//butano
#include "bn_core.h"
#include "bn_version.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_display.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_affine_bg_pivot_position_hbe_ptr.h"
#include "bn_affine_bg_attributes_hbe_ptr.h"

//ds code
#include "ds_scene.h"



//assets
#include "bn_regular_bg_items_plash.h"

#include "bn_music_items.h"

#include "bn_sprite_text_generator.h"
#include "variable_8x8_sprite_font.h"

//sound
#include "bn_sound_items.h"

namespace ds
{

Scene Splash::execute()
{
    
    bn::regular_bg_ptr map = bn::regular_bg_items::plash.create_bg(0 , 0);
    bn::sound_items::sfx_bling.play(1);
    int timer = 0;
    while(true)
    {
        if(timer > 180){
            return Scene::BUTANO;
        }
        
        ++timer;
        bn::core::update();
    }   
}
}