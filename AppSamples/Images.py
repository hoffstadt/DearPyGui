from marvel import *
import marvel_constants as mc

add_input_float2("uv_min", default_value = (0, 0), callback="Update")
add_input_float2("uv_max", default_value = (1, 1), callback="Update" )

add_data("TextureCoordinates", [0, 0, 1, 1])

def Update(sender, data):

    uvmin = get_value("uv_min")
    uvmax = get_value("uv_max")
    uvminx = uvmin[0]
    uvminy = uvmin[1]
    uvmaxx = uvmax[0]
    uvmaxy = uvmax[1]

    add_data("TextureCoordinates", [uvminx, uvminy, uvmaxx, uvmaxy])

    print(get_data("TextureCoordinates"))
    

add_image("Full", "SpriteMapExample.png")
add_image("Partial", "SpriteMapExample.png", secondary_data_source="TextureCoordinates")