#include "ds_scene_file.h"

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
#include "bn_regular_bg_items_filescreen_bg.h"
#include "bn_regular_bg_items_tit_1.h"
#include "bn_sprite_items_cursor.h"


//music
#include "bn_music_items.h"
#include "bn_music_actions.h"

//sound
#include "bn_sound_items.h"

namespace ds
{
    
        constexpr bn::fixed text_y_inc = 20;
        constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

        Scene File::execute()
        {

            bn::regular_bg_ptr bg0 = bn::regular_bg_items::tit_1.create_bg(0, 0);
            //fade in background
            bn::regular_bg_ptr background = bn::regular_bg_items::filescreen_bg.create_bg(0, 0);
            
            bool sel_movement1 = true;
            bool sel_movement = false;
            bool sel_box = false;

            bg0.set_visible(true);
            background.set_visible(true);
            background.set_blending_enabled(true);
            bn::blending::set_fade_alpha(1);
    
            bn::blending_fade_alpha_to_action action(30,0);

            while(!action.done())
            {
                action.update();
                bn::core::update();
            }

            int framesToWait=30*2;

            while(framesToWait--)
            {
                bn::core::update();
            }

            //Start FILE MUSIC
            bn::music_items::bgm_file.play(1);
            bn::music::set_volume(0.5);
            
            //TEXT
            bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

            bn::vector<bn::sprite_ptr, 32> text_sprites;
            bn::vector<bn::sprite_ptr, 32> text_sprites_1;
            bn::vector<bn::sprite_ptr, 32> text_sprites_2;
            bn::vector<bn::sprite_ptr, 32> text_sprites_3;
            bn::vector<bn::sprite_ptr, 32> text_blinking;
        
            text_generator.generate(-55, -10, "GAME", text_sprites_1);
            text_generator.generate(-55, 0, "BUTTON TEST", text_sprites_2);
            text_generator.generate(-55, 10, "-> BACK TO TITLE", text_sprites_3);
            text_generator.generate(-41, -53, "SELECT MODE", text_sprites);

            bn::sprite_ptr cursor = bn::sprite_items::cursor.create_sprite(-75,-10,0);
            
            unsigned int time = 0;
            bool alwaysvisible = (time++ % 1) < 1;
            while (!bn::keypad::a_pressed())
            {
                if(sel_movement1 == true)
                {
                    text_blinking = text_sprites_1;

                    if(bn::keypad::down_pressed())
                    {   
                        text_blinking = text_sprites_2;

                        cursor.set_position(-75,0);

                        bn::sound_items::sfx_cursor.play(1);
                        
                        sel_movement = true;
                        sel_movement1 = false;
                        sel_box = false;
                       for ( auto spr : text_sprites_1)
                       {
                        spr.set_visible(alwaysvisible);
                       }
                    }  
                }

                else if(sel_movement == true)
                {
                    if(bn::keypad::up_pressed())
                    {   
                        text_blinking = text_sprites_1;

                        cursor.set_position(-75,-10);

                        bn::sound_items::sfx_cursor.play(1);

                        sel_movement1 = true;
                        sel_movement = false;
                        sel_box = false;
                        for (auto spr : text_sprites_2)
                        {
                        spr.set_visible(alwaysvisible);
                        }
                    }

                    if(bn::keypad::down_pressed())
                    {   
                        text_blinking = text_sprites_3;

                        cursor.set_position(-75,10);

                        bn::sound_items::sfx_cursor.play(1);

                        sel_box = true;
                        sel_movement = false;
                        sel_movement1 = false;
                        for (auto spr : text_sprites_2)
                        {
                        spr.set_visible(alwaysvisible);
                        }
                    }
                }

                else if(sel_box == true)
                {
                    if(bn::keypad::up_pressed())
                    {   
                        text_blinking = text_sprites_2;

                        cursor.set_position(-75,0);

                        bn::sound_items::sfx_cursor.play(1);

                        sel_movement = true;
                        sel_box = false;
                        sel_movement1 = false;
                        for (auto spr : text_sprites_3)
                        {
                        spr.set_visible(alwaysvisible);
                        }
                    }
                }

                //Blinking TEXT!!!
                bool visible = (time++ % 16) < 8;
    
                for (auto spr : text_blinking)
                {
                    spr.set_visible(visible);
                }
    
                bn::core::update();   
            
            if(bn::keypad::a_pressed())
                {
                      //Stop FILE MUSIC
                      bn::music_items::bgm_file.play(0);

                      if (sel_movement)
                      {
                        return Scene::BUTTONTEST;
                      }
                      else if (sel_movement1)
                      {
                        return Scene::GAME;
                      }
                      else if (sel_box)
                      {
                        return Scene::TITLE;
                      }
                }
            }
        }
}