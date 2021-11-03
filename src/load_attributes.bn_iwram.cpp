/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "load_attributes.h"

void load_attributes(bn::affine_bg_mat_attributes base_attributes, bn::affine_bg_mat_attributes* attributes)
{
    bn::fixed scale = base_attributes.horizontal_scale();

    for(int index = 0; index < bn::display::height(); ++index)
    {
        base_attributes.set_scale(scale);
        scale += 0.003;
        attributes[index] = base_attributes;
    }
}
