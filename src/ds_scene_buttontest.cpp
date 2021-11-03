#include "ds_scene_buttontest.h"

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
#include "bn_regular_bg_attributes_hbe_ptr.h"
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
#include "bn_sprite_items_bongup.h"
#include "bn_sprite_items_bongdownleft.h"
#include "bn_sprite_items_bongdownright.h"


//backgrounds
#include "bn_regular_bg_items_buttontest_bg.h"
#include "bn_regular_bg_items_button_overlay.h"
#include "bn_regular_bg_items_bong0.h"



//#include "bn_sprite_items_btn_cross_s.h"
#include "bn_sprite_items_btn_up_s.h"
#include "bn_sprite_items_btn_down_s.h"
#include "bn_sprite_items_btn_left_s.h"
#include "bn_sprite_items_btn_right_s.h"
#include "bn_sprite_items_btn_ab_s.h"
#include "bn_sprite_items_btn_l_s.h"
#include "bn_sprite_items_btn_r_s.h"



//music
#include "bn_music_items.h"
#include "bn_music_actions.h"

//sound
#include "bn_sound_items.h"

namespace ds
{
    
        constexpr bn::fixed text_y_inc = 20;
        constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

        Scene Buttontest::execute()
        {

                bool _key_notheld = true;
                bool bongoright = false;
                bool bongoleft = false;
                bool bongoboth = false;
                bool bongonone = true;

                

                //Start FILE MUSIC
                bn::music_items::bgm_90s.play(1);
                bn::music::set_volume(0.5);

                //fade in background
                bn::regular_bg_ptr background = bn::regular_bg_items::buttontest_bg.create_bg(0, 0);
                bn::regular_bg_ptr overlay = bn::regular_bg_items::button_overlay.create_bg(0, 0);
                bn::regular_bg_ptr b0 = bn::regular_bg_items::bong0.create_bg(+10, +2);

                overlay.set_visible(true);

                b0.set_visible(false);
                

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

                
                background.set_blending_enabled(false);
                b0.set_visible(true);
                b0.set_blending_enabled(true);
               
                bn::array<bn::fixed, bn::display::height()> horizontal_deltas;

                bn::regular_bg_position_hbe_ptr horizontal_position_hbe =
                bn::regular_bg_position_hbe_ptr::create_horizontal(b0, horizontal_deltas);
                bn::fixed base_degrees_angle;

                bn::blending::set_fade_alpha(1);
                
                action = bn::blending_fade_alpha_to_action(40,0);
                
                while(!action.done())  
                    {
                        base_degrees_angle += 4;

                        if(base_degrees_angle >= 360)
                        {
                            base_degrees_angle -= 360;
                        }

                        bn::fixed degrees_angle = base_degrees_angle;

                        for(int index = 0, limit = bn::display::height() / 2; index < limit; ++index)
                        {
                            degrees_angle += 16;

                            if(degrees_angle >= 360)
                            {
                                degrees_angle -= 360;
                            }

                            bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 6;
                            horizontal_deltas[(bn::display::height() / 2) + index] = desp;
                            horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
                        }

                        horizontal_position_hbe.reload_deltas_ref();
                        action.update();
                        bn::core::update();

                    }

                    b0.set_visible(false);
                    bn::regular_bg_ptr b1 = bn::regular_bg_items::bong0.create_bg(+10, +2);
                

                
                //buttons
                //bn::sprite_ptr btn_cross_s_sprite = bn::sprite_items::btn_cross_s.create_sprite(-86, -9);//#
                bn::sprite_ptr btn_up = bn::sprite_items::btn_up_s.create_sprite(-101, -9);//#
                bn::sprite_ptr btn_down = bn::sprite_items::btn_down_s.create_sprite(-101, -9);//#
                bn::sprite_ptr btn_left = bn::sprite_items::btn_left_s.create_sprite(-101, -9);//#
                bn::sprite_ptr btn_right = bn::sprite_items::btn_right_s.create_sprite(-101, -9);//#

                bn::sprite_ptr btn_b = bn::sprite_items::btn_ab_s.create_sprite(+94, -6);//#
                bn::sprite_ptr btn_a = bn::sprite_items::btn_ab_s.create_sprite(+108, -16);
                bn::sprite_ptr btn_l = bn::sprite_items::btn_l_s.create_sprite(-103, -41);//#
                bn::sprite_ptr btn_r = bn::sprite_items::btn_r_s.create_sprite(+103, -41);//#

                //paws up
                bn::sprite_ptr bupa = bn::sprite_items::bongup.create_sprite(-55, -7);//#
                bn::sprite_ptr bupb = bn::sprite_items::bongup.create_sprite(+37, +14);//#

                //pawsdown
                bn::sprite_ptr bdownleft = bn::sprite_items::bongdownleft.create_sprite(-54, +18);//#
                bn::sprite_ptr bdownright = bn::sprite_items::bongdownright.create_sprite(+26, +50);//#



                while (true) //Input
                { 
                    btn_up.set_visible(false);
                    btn_down.set_visible(false);
                    btn_left.set_visible(false);
                    btn_right.set_visible(false);
                    btn_a.set_visible(false);
                    btn_b.set_visible(false);
                    btn_l.set_visible(false);
                    btn_r.set_visible(false);

                    bupa.set_visible(true);
                    bupb.set_visible(true);
                    bdownleft.set_visible(false);
                    bdownright.set_visible(false);

                    _key_notheld = true;

                    if (bn::keypad::up_held())
                    {
                        btn_up.set_visible(true);
                        _key_notheld = false;
                    }

                    if (bn::keypad::down_held())
                    {
                        btn_down.set_visible(true);
                        _key_notheld = false;
                    }

                    if (bn::keypad::left_held())
                    {
                        btn_left.set_visible(true);
                        _key_notheld = false;
                    }

                    if (bn::keypad::right_held())
                    {
                        btn_right.set_visible(true);
                        _key_notheld = false;
                    }

                    if (bn::keypad::a_held())
                    {
                        btn_a.set_visible(true);
                        _key_notheld = false;
                        bongoleft = true;
                        bupa.set_visible(true);
                        bupb.set_visible(false);
                        bdownleft.set_visible(false);
                        bdownright.set_visible(true);
                        
                    }

                    if (bn::keypad::b_held())
                    {
                        btn_b.set_visible(true);
                        _key_notheld = false;
                        bongoright = true;
                        bupa.set_visible(false);
                        bupb.set_visible(true);
                        bdownleft.set_visible(true);
                        bdownright.set_visible(false);
                    }

                    if (bn::keypad::l_held())
                    {
                        btn_l.set_visible(true);
                        _key_notheld = false;
                        bongoleft = true;
                        bupa.set_visible(false);
                        bupb.set_visible(true);
                        bdownleft.set_visible(true);
                        bdownright.set_visible(false);

                    }

                    if (bn::keypad::r_held())
                    {
                        btn_r.set_visible(true);
                        _key_notheld = false;
                        bongoright = true;
                        bupa.set_visible(true);
                        bupb.set_visible(false);
                        bdownleft.set_visible(false);
                        bdownright.set_visible(true);
                    }

                    if ((bn::keypad::a_held() && bn::keypad::b_held()) || (bn::keypad::l_held() && bn::keypad::r_held()))
                    {
                        bongoboth = true;
                        _key_notheld = false;
                        bupa.set_visible(false);
                        bupb.set_visible(false);
                        bdownleft.set_visible(true);
                        bdownright.set_visible(true);

                    }

                    if (bn::keypad::a_pressed() || bn::keypad::l_pressed())
                    {
                        
                        bn::sound_items::sfx_hit_s.play(0.8, 1.5, 0);
                    }

                    if (bn::keypad::b_pressed()  || bn::keypad::r_pressed())
                    {
                        
                        bn::sound_items::sfx_hit_l.play(0.8, 1.5, 0);
                        
                    }
                    
                    

                    //GO BACK TO MENU
                    if (bn::keypad::select_pressed())
                    {
                           //Stop FILE MUSICn
                           bn::music_items::bgm_90s.play(0);
                           return Scene::FILE;
                    }
                                bn::core::update();            
                              
                }    
                
                
                    
                
                            
        } 
             
}