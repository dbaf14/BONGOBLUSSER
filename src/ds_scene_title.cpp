#include "ds_scene_title.h"

//butano
#include "bn_core.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_string.h"
#include "bn_display.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_affine_bg_attributes_hbe_ptr.h"
#include "bn_blending_actions.h"
#include "bn_blending_fade_alpha.h"
#include "bn_sprite_text_generator.h"
#include "bn_blending_fade_alpha_hbe_ptr.h"
#include "bn_blending_transparency_attributes.h"
#include "bn_blending_transparency_attributes_hbe_ptr.h"

//ds code
#include "ds_scene.h"


//text
#include "common_fixed_8x8_sprite_font.h"
#include "common_fixed_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "bn_sprite_items_variable_8x16_font_yellow.h"
#include "fixed_32x64_sprite_font.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_ptr.h"

//assets
#include "bn_regular_bg_items_bg_1.h"
#include "bn_regular_bg_items_tit_1.h"
#include "bn_regular_bg_items_logo_1.h"
#include "bn_regular_bg_items_tit_1_s.h"
#include "bn_sprite_items_coin_s.h"
#include "bn_sprite_items_sp1.h"

//music
#include "bn_music_items.h"
#include "bn_sound_items.h"

namespace ds
{
    bn::fixed loop(bn::fixed number, bn::fixed increment)
    {
        number += increment;
        if(number > 256){
            return 0;
        } else {
            return number;
        }
    }


    auto source_1 = bn::regular_bg_items::logo_1;
    auto source_2 = bn::regular_bg_items::bg_1;
    
    

    unsigned int big_number = 64;
    unsigned int small_number = 32;

    constexpr bn::fixed text_y_inc = 10;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    Scene Title::execute()
    {
            //first screen
            bn::regular_bg_ptr source1 = source_1.create_bg(0, 0);
            bn::regular_bg_ptr source2 = source_2.create_bg(0, 0);
            bn::sprite_ptr sp1_sprite = bn::sprite_items::sp1.create_sprite(+30, +30);
            sp1_sprite.set_visible(false);
            sp1_sprite.set_scale(2);
            bool aniok = true;

          

            bool abort = false;
            source2.set_visible(true);
            source1.set_blending_enabled(true);
            bn::blending::set_fade_alpha(1);
    
            bn::blending_fade_alpha_to_action action(60,0);

            while(!action.done() && !abort)
            {
                action.update();
                bn::core::update();
                if(bn::keypad::start_pressed())
                    abort = true;
            }

            int framesToWait=110*2;
            bn::sound_items::sfx_hit_s.play(0.8, 1.5, 0);
            while(framesToWait-- && !abort)
            {
                //SHOW PRO LOGO
                sp1_sprite.set_visible(true);
                
                sp1_sprite.set_blending_enabled(true);
                bn::core::update();
                if(bn::keypad::start_pressed())
                    abort = true;
            }

            action = bn::blending_fade_alpha_to_action(80,1);

            while(!action.done() && !abort)
            {
                action.update();
                bn::core::update();
                if(bn::keypad::start_pressed())
                    abort = true;
            }
            sp1_sprite.set_visible(false);
            source2.set_visible(false);
            source1.set_visible(false);
            bn::blending::set_fade_alpha(0);
            //second screen

            //coin animation
            bn::sprite_ptr coin_s_sprite = bn::sprite_items::coin_s.create_sprite(+75, -30);
            coin_s_sprite.set_scale(0.80);
            bn::sprite_animate_action<8> coin_s_animate_action = bn::create_sprite_animate_action_forever(
                coin_s_sprite, 6, bn::sprite_items::coin_s.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);

            
                
                //


            source_1 = bn::regular_bg_items::tit_1;
            //??????
            bn::regular_bg_ptr source3 = source_1.create_bg(0, 0);
        
            abort = false;
            source3.set_visible(true);
            source3.set_blending_enabled(true);
        
            //Start TITLE MUSIC
            bn::music_items::title_music.play(0.5);

            bn::blending::set_fade_alpha(1);

            action = bn::blending_fade_alpha_to_action(90,0);
            

            while(!action.done() && !abort)
            {
                action.update();
                coin_s_animate_action.update();
                bn::core::update();
                if(bn::keypad::start_pressed())
                    abort = true;
            }

            framesToWait = 60*2;

            while(framesToWait-- && !abort)
            {
                bn::core::update();
                coin_s_animate_action.update();
                if(bn::keypad::start_pressed())
                    abort = true;
            }

            bn::blending::set_fade_alpha(0);
    
            //third screen
            
            source_2 = bn::regular_bg_items::tit_1_s;
            
            bn::regular_bg_ptr source4 = source_2.create_bg(0, 0);
            source4.set_visible(true);
            source3.set_blending_enabled(false);
            //TEXT
            bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

            bn::vector<bn::sprite_ptr, 32> text_sprites;
            bn::vector<bn::sprite_ptr, 32> text_sprites_1;

        
            text_generator.generate(-60, text_y_limit, "PRESS START BUTTON", text_sprites_1);
            text_generator.generate(-105, -text_y_limit, "  2021 pr0gramm Games / dbaf14", text_sprites);
            
            
            
            source4.set_blending_enabled(true);

            bn::blending::set_fade_alpha(1);

            action = bn::blending_fade_alpha_to_action(30,0);

            while(!action.done())
            {
                coin_s_animate_action.update();
                action.update();
                bn::core::update();
                
            }

            framesToWait = 30*2;

            while(framesToWait--)
            {
                coin_s_animate_action.update();
                bn::core::update();  
            }

            bn::blending::set_fade_alpha(0);

            //Blinking TEXT!!!
            unsigned int time = 0;
            

            while (!bn::keypad::start_pressed())
            {
                bool visible = (time++ % big_number) < small_number;
    
                for (auto spr : text_sprites_1)
                {
                    spr.set_visible(visible);
                }
                   coin_s_animate_action.update();
                 bn::core::update();
                 
            }

            if(bn::keypad::start_pressed())
            {
                   
                source_1 = bn::regular_bg_items::logo_1;
                source_2 = bn::regular_bg_items::bg_1;
                

                framesToWait = 20*2;

                    while(framesToWait--)
                    {
                        coin_s_animate_action.update();
                        bn::core::update();  
                    }

                    //Stop TITLE MUSIC
                  bn::music_items::title_music.play(0);
                  return Scene::FILE;
            }
            
                //bn::core::init();
    }
}