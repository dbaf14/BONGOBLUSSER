#include "ds_scene_game.h"

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
#include "bn_sprite_actions.h"
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
#include "bn_sprite_items_righthit.h"
#include "bn_sprite_items_lefthit.h"
#include "bn_sprite_items_plus.h"
#include "bn_sprite_items_minus.h"
#include "bn_sprite_items_scorebg.h"
#include "bn_sprite_items_combo.h"

//backgrounds
#include "bn_regular_bg_items_buttontest_bg.h"
#include "bn_regular_bg_items_bong0.h"

//music
#include "bn_music_items.h"
#include "bn_music_actions.h"

//sound
#include "bn_sound_items.h"

namespace ds
{
        constexpr bn::fixed text_y_inc = 20;
        constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

        Scene Game::execute()
        {
                bool makesoundr = false;
                bool makesoundl = false;
                bool scoreplus = false;
                bool scoreminus = false;
                bool comb = false;
                bool finish = false;
                int score = 0;
                int combo = 0;


                
                //POSITIONS FOR LEFT COINS
                int start_xl = -130;
                int start_yl = +8;

                int score_xl = -60;
                int score_yl = +21;

                int finish_xl = -35;
                int finish_yl = +27;

                //POSITIONS FOR RIGHT COINS
                int start_xr = +136;
                int start_yr = +65;
                
                int score_xr = +22;
                int score_yr = +41;

                int finish_xr = +1;
                int finish_yr = +35;

                int duration_updates = 30;

                //Start GAME MUSIC
                bn::music_items::soulom.play(1);
                bn::music::set_volume(0.5);

                //fade in background
                bn::regular_bg_ptr background = bn::regular_bg_items::buttontest_bg.create_bg(0, 0);
                bn::regular_bg_ptr b0 = bn::regular_bg_items::bong0.create_bg(+10, +2);
                b0.set_visible(false);
                background.set_visible(true);
                background.set_blending_enabled(true);
                bn::blending::set_fade_alpha(1);
    
                bn::blending_fade_alpha_to_action action(40,0);

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
               
                //START H-Blank effect on Background
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

                    //old background is not visible anymore
                    b0.set_visible(false);
                    //create same background(Pure) as before on same position but not deformed through H-Blank effect
                    bn::regular_bg_ptr b1 = bn::regular_bg_items::bong0.create_bg(+10, +2);

                //CREATE SPRITES
                //paws up
                bn::sprite_ptr bupa = bn::sprite_items::bongup.create_sprite(-55, -7);//#
                bn::sprite_ptr bupb = bn::sprite_items::bongup.create_sprite(+37, +14);//#

                //pawsdown
                bn::sprite_ptr bdownleft = bn::sprite_items::bongdownleft.create_sprite(-54, +18);//#
                bn::sprite_ptr bdownright = bn::sprite_items::bongdownright.create_sprite(+26, +50);//#

                //pawsdown hit effect
                bn::sprite_ptr hitleft = bn::sprite_items::lefthit.create_sprite(-54, +18);//#
                bn::sprite_ptr hitright = bn::sprite_items::righthit.create_sprite(+26, +50);//#

                //Plus and Minus Coins for TEST
                //bn::sprite_ptr plus = bn::sprite_items::plus.create_sprite(-60, +21);//# = RIGHT SCORE POSITION
                //bn::sprite_ptr minus = bn::sprite_items::minus.create_sprite(+22, +41);//# =LEFT SCORE POSITION
                bn::sprite_ptr plusl1 = bn::sprite_items::plus.create_sprite(-130, +8);//# =LEFT SPRITE START POSITION
                bn::sprite_ptr minusl1 = bn::sprite_items::plus.create_sprite(-130, +8);//# =LEFT SPRITE START POSITION
                bn::sprite_ptr plusr1 = bn::sprite_items::minus.create_sprite(+136, +65);//# =RIGHT SPRITE START POSITION
                bn::sprite_ptr minusr1 = bn::sprite_items::minus.create_sprite(+136, +65);//# =RIGHT SPRITE START POSITION

                //Score Background
                bn::sprite_ptr scorebg = bn::sprite_items::scorebg.create_sprite(-12, -73);//# =SCORE Display Background
                scorebg.set_scale(2);

                //combo sign
                bn::sprite_ptr combos = bn::sprite_items::combo.create_sprite(+100, -63);//# =SCORE Display Background
                combos.set_scale(0.5);
                //TEXT
                bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
                bn::vector<bn::sprite_ptr, 32> text_sprites;
                bn::string<32> score_numb("SCORE: ");
                score_numb.append(bn::to_string<4>(score));
                text_generator.generate(-60, -65, score_numb, text_sprites);

                

                //INPUT
                while (true) 
                { 
                    bupa.set_visible(true);
                    bupb.set_visible(true);
                    bdownleft.set_visible(false);
                    bdownleft.put_above();
                    bdownright.set_visible(false);
                    bdownright.put_above();
                    hitleft.set_visible(false);
                    hitright.set_visible(false);
                    combos.set_visible(false);
                    finish = false;

                    plusl1.put_below();
                    minusl1.put_below();

                    plusr1.put_below();
                    minusr1.put_below();

                    //score = 0;

                    makesoundr = false;
                    makesoundl = false;


                    if(score <= -1)
                    {
                        score = 0;
                        text_sprites.clear();
                            score_numb = "SCORE: ";
                            score_numb.append(bn::to_string<4>(score));
                            text_generator.generate(-60, -65, score_numb, text_sprites);
                    }

                    if(combo >= 5)
                    {
                        combo == 5;

                        combos.set_visible(true);
                        comb = true;
                       
                        
                    }
                    if(combo < 5)
                    {
                        
                        combos.set_visible(false);
                    }

                    

                    //A||R KEY for right Paw
                    if (!bn::keypad::r_held() && bn::keypad::a_held())  
                    {
                        bupb.set_visible(false);
                        bdownright.set_visible(true);
                        makesoundr = true;
                    }
                    //A||R KEY for right Paw
                    if (!bn::keypad::a_held() && bn::keypad::r_held()) 
                    {
                        bupb.set_visible(false);
                        bdownright.set_visible(true);
                        makesoundr = true;
                    }
                    //A+R KEY for right Paw 
                    if (bn::keypad::a_held() && bn::keypad::r_held()) 
                    {
                        bupb.set_visible(false);
                        bdownright.set_visible(true);
                    }
                    //B||L KEY for left Paw
                    if (!bn::keypad::l_held() && bn::keypad::b_held()) 
                    {
                        bupa.set_visible(false);
                        bdownleft.set_visible(true);
                        makesoundl = true;
                    }
                    //B||L KEY for left Paw
                    if (!bn::keypad::b_held() && bn::keypad::l_held()) 
                    {
                        bupa.set_visible(false);
                        bdownleft.set_visible(true);
                        makesoundl = true;
                    }
                    //B+L KEY for left Paw
                    if (bn::keypad::b_held() && bn::keypad::l_held()) 
                    {
                        bupa.set_visible(false);
                        bdownleft.set_visible(true);
                    }




                    //Play right hitsound and show the hiteffect shortly, Remove plus||minus Coin
                    if (makesoundr && (bn::keypad::a_pressed() || bn::keypad::r_pressed())) 
                    {
                        bn::sound_items::sfx_hit_s.play(0.8, 1.5, 0);
                        hitright.set_visible(true);
                        makesoundr = false;

                        
                        //TEST REMOVING COINS AND ADD/REMOVE SCOREPOINT (+22, +41)
                        if((plusr1.x() > +16 && plusr1.x() < +28) && (plusr1.y() > +35 && plusr1.y() < +47)) //HITRANGE
                        {
                            plusr1.set_visible(false);
                            score = score -1;
                            combo = 0;

                            text_sprites.clear();
                            score_numb = "SCORE: ";
                            score_numb.append(bn::to_string<4>(score));
                            text_generator.generate(-60, -65, score_numb, text_sprites);
                            
                        }
                        if((minusr1.x() > +16 && minusr1.x() < +28) && (minusr1.y() > +35 && minusr1.y() < +47)) //HITRANGE
                        {
                            minusr1.set_visible(false);
                            score = score +1;
                            combo = combo +1;
                                                    if (comb == true)
                                                    {
                                                    bn::sound_items::sfx_bling.play(0.5, 1.5, 0);
                                                    score = score +10;
                                                    comb = false;
                                                    }
                            text_sprites.clear();
                            score_numb = "SCORE: ";
                            score_numb.append(bn::to_string<4>(score));
                            text_generator.generate(-60, -65, score_numb, text_sprites);
                            
                        }
                        
                    }
                    //Play left hitsound and show the hiteffect shortly, Remove plus||minus Coin
                    if (makesoundl && (bn::keypad::b_pressed() || bn::keypad::l_pressed())) 
                    {
                        bn::sound_items::sfx_hit_l.play(0.8, 1.5, 0);
                        hitleft.set_visible(true);
                        makesoundl = false;

                        
                        
                        //TEST REMOVING COINS AND ADD/REMOVE SCOREPOINT +21,-60
                        if((plusl1.y() > +13 && plusl1.y() < +28) && (plusl1.x() < -52 && plusl1.x() > -69)) //HITRANGE
                        {
                            plusl1.set_visible(false);
                            score = score -1;
                            combo = 0;
                            text_sprites.clear();
                            score_numb = "SCORE: ";
                            score_numb.append(bn::to_string<4>(score));
                            text_generator.generate(-60, -65, score_numb, text_sprites);
                            
                        }
                        if((minusl1.y() > +13 && minusl1.y() < +28) && (minusl1.x() < -52 && minusl1.x() > -69)) //HITRANGE
                        {
                            minusl1.set_visible(false);
                            score = score +1;
                            combo = combo +1;
                                                if (comb == true)
                                                {
                                                bn::sound_items::sfx_bling.play(0.5, 1.5, 0);
                                                score = score +10;
                                                comb = false;
                                                }
                            text_sprites.clear();
                            score_numb = "SCORE: ";
                            score_numb.append(bn::to_string<4>(score));
                            text_generator.generate(-60, -65, score_numb, text_sprites);

                        }
                        
                    }


                    if(!finish)
                    {
                    //MOVING +/- SPRITES
                    bn::sprite_move_loop_action _moveactionl1(plusr1,duration_updates, finish_xl, finish_yl);
                    bn::sprite_move_loop_action _moveactionr1(minusl1,duration_updates, finish_xr, finish_yr);

                    
                    _moveactionl1.update();
                     _moveactionr1.update(); 
                    }

                    //REMOVE MISSED COINS LEFT -35, +27
                    if(plusl1.x() > -39  && plusl1.y() < +37 ) //HITRANGE
                        {
                            plusl1.set_visible(false);
                            //bn::sound_items::sfx_miss.play(0.8, 1.5, 0);
                            plusl1.set_position(start_xl,start_yl);
                            plusl1.set_visible(true);
                            combo = 0;
                        }

                    if(minusl1.x() > -39  && minusl1.y() < +37 ) //HITRANGE
                        {
                            minusl1.set_visible(false);
                            //bn::sound_items::sfx_miss.play(0.8, 1.5, 0);
                            minusl1.set_position(start_xl,start_yl);
                            minusl1.set_visible(true);
                            
                        }

                    //REMOVE MISSED COINS RIGHT +1, +35
                    if(plusr1.x() > 0  && plusr1.y() < +36 ) //HITRANGE
                        {
                            plusr1.set_visible(false);
                            //bn::sound_items::sfx_miss.play(0.8, 1.5, 0);
                            plusr1.set_position(start_xr,start_yr);
                            plusr1.set_visible(true);
                            combo = 0;
                        }

                    if(minusr1.x() > 0  && minusr1.y() < +36 ) //HITRANGE
                        {
                            minusr1.set_visible(false);
                            //bn::sound_items::sfx_miss.play(0.8, 1.5, 0);
                            minusr1.set_position(start_xr,start_yr);
                            minusr1.set_visible(true);
                           
                        }



                    
                    

                                        //GO BACK TO MENU // LATER FOR PAUSE-MENU?
                                        if (bn::keypad::select_pressed())
                                        {
                                               //Stop GAME MUSIC
                                               bn::music_items::soulom.play(0);
                                               return Scene::FILE;
                                        }
                     bn::core::update();

                     
                }
        bn::core::update();
        }       
}