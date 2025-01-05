function init()
    print("Level 1 init!")
end

function update()
    print("Level 1 update!")
end




-- Define a table with the start values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] = { type = "texture", id = "tilemap-texture", path = "tilemaps/jungle.png" },
        { type = "texture", id = "chopper-texture", path = "images/chopper-green-spritesheet.png" },
        { type = "texture", id = "su27-texture", path = "images/su27-spritesheet.png" },
        { type = "texture", id = "f22-texture", path = "images/f22-spritesheet.png" },
        { type = "texture", id = "fw190-texture", path = "images/fw190-spritesheet.png" },
        { type = "texture", id = "upf7-texture", path = "images/upf7-spritesheet.png" },
        { type = "texture", id = "bf109-texture", path = "images/bf109-spritesheet.png" },
        { type = "texture", id = "bomber-texture", path = "images/bomber-spritesheet.png" },
        { type = "texture", id = "carrier-texture", path = "images/carrier.png" },
        { type = "texture", id = "boat-texture", path = "images/boat.png" },
        { type = "texture", id = "tank-tiger-up-texture", path = "images/tank-tiger-up.png" },
        { type = "texture", id = "tank-tiger-right-texture", path = "images/tank-tiger-right.png" },
        { type = "texture", id = "tank-tiger-down-texture", path = "images/tank-tiger-down.png" },
        { type = "texture", id = "tank-tiger-left-texture", path = "images/tank-tiger-left.png" },
        { type = "texture", id = "tank-tiger-killed-texture", path = "images/tank-tiger-killed.png" },
        { type = "texture", id = "tank-panther-up-texture", path = "images/tank-panther-up.png" },
        { type = "texture", id = "tank-panther-right-texture", path = "images/tank-panther-right.png" },
        { type = "texture", id = "tank-panther-down-texture", path = "images/tank-panther-down.png" },
        { type = "texture", id = "tank-panther-left-texture", path = "images/tank-panther-left.png" },
        { type = "texture", id = "tank-panther-killed-texture", path = "images/tank-panther-killed.png" },
        { type = "texture", id = "truck-ford-up-texture", path = "images/truck-ford-up.png" },
        { type = "texture", id = "truck-ford-right-texture", path = "images/truck-ford-right.png" },
        { type = "texture", id = "truck-ford-down-texture", path = "images/truck-ford-down.png" },
        { type = "texture", id = "truck-ford-left-texture", path = "images/truck-ford-left.png" },
        { type = "texture", id = "truck-ford-killed-texture", path = "images/truck-ford-killed.png" },
        { type = "texture", id = "army-walk-up-texture", path = "images/army-walk-up.png" },
        { type = "texture", id = "army-walk-right-texture", path = "images/army-walk-right.png" },
        { type = "texture", id = "army-walk-down-texture", path = "images/army-walk-down.png" },
        { type = "texture", id = "army-walk-left-texture", path = "images/army-walk-left.png" },
        { type = "texture", id = "army-walk-killed-texture", path = "images/army-walk-killed.png" },
        { type = "texture", id = "army-gun-up-texture", path = "images/army-gun-up.png" },
        { type = "texture", id = "army-gun-right-texture", path = "images/army-gun-right.png" },
        { type = "texture", id = "army-gun-down-texture", path = "images/army-gun-down.png" },
        { type = "texture", id = "army-gun-left-texture", path = "images/army-gun-left.png" },
        { type = "texture", id = "sam-truck-right-texture", path = "images/sam-truck-right.png" },
        { type = "texture", id = "sam-tank-left-texture", path = "images/sam-tank-left-spritesheet.png" },
        { type = "texture", id = "sam-tank-right-texture", path = "images/sam-tank-right-spritesheet.png" },
        { type = "texture", id = "takeoff-base-texture", path = "images/takeoff-base.png" },
        { type = "texture", id = "landing-base-texture", path = "images/landing-base.png" },
        { type = "texture", id = "runway-texture", path = "images/runway.png" },
        { type = "texture", id = "obstacles1-texture", path = "images/obstacles-1.png" },
        { type = "texture", id = "obstacles2-texture", path = "images/obstacles-2.png" },
        { type = "texture", id = "obstacles3-texture", path = "images/obstacles-3.png" },
        { type = "texture", id = "obstacles4-texture", path = "images/obstacles-4.png" },
        { type = "texture", id = "obstacles5-texture", path = "images/obstacles-5.png" },
        { type = "texture", id = "obstacles6-texture", path = "images/obstacles-6.png" },
        { type = "texture", id = "obstacles7-texture", path = "images/obstacles-7.png" },
        { type = "texture", id = "tree1-texture", path = "images/tree-1.png" },
        { type = "texture", id = "tree2-texture", path = "images/tree-2.png" },
        { type = "texture", id = "tree3-texture", path = "images/tree-3.png" },
        { type = "texture", id = "tree4-texture", path = "images/tree-4.png" },
        { type = "texture", id = "tree5-texture", path = "images/tree-5.png" },
        { type = "texture", id = "tree6-texture", path = "images/tree-6.png" },
        { type = "texture", id = "tree7-texture", path = "images/tree-7.png" },
        { type = "texture", id = "tree8-texture", path = "images/tree-8.png" },
        { type = "texture", id = "tree9-texture", path = "images/tree-9.png" },
        { type = "texture", id = "tree10-texture", path = "images/tree-10.png" },
        { type = "texture", id = "tree11-texture", path = "images/tree-11.png" },
        { type = "texture", id = "tree12-texture", path = "images/tree-12.png" },
        { type = "texture", id = "tree13-texture", path = "images/tree-13.png" },
        { type = "texture", id = "tree14-texture", path = "images/tree-14.png" },
        { type = "texture", id = "tree15-texture", path = "images/tree-15.png" },
        { type = "texture", id = "tree16-texture", path = "images/tree-16.png" },
        { type = "texture", id = "tree17-texture", path = "images/tree-17.png" },
        { type = "texture", id = "tree18-texture", path = "images/tree-18.png" },
        { type = "texture", id = "tree19-texture", path = "images/tree-19.png" },
        { type = "texture", id = "tree20-texture", path = "images/tree-20.png" },
        { type = "texture", id = "bullet-texture", path = "images/bullet.png" },
        { type = "texture", id = "radar-texture", path = "images/radar-spritesheet.png" },
        { type = "font", id = "pico8-font-5", path = "fonts/pico8.ttf", font_size = 5 },
        { type = "font", id = "pico8-font-10", path = "fonts/pico8.ttf", font_size = 10 }
    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    tilemap = {
        path = "tilemaps/jungle.map",
        texture_asset_id = "tilemap-texture",
        num_rows = 10,
        num_cols = 10,
        tile_size = 32,
        scale = 2.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = {
        [0] = {
            -- F-22 fighter jet
            group = "chopper-texture",
            components = {
                transform = {
                    position = { x = 5, y = 5 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "tree1-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                --animation = {
                --    num_frames = 1,
                --    speed_rate = 10 -- fps
                --},
                boxcollider = {
                    width = 32,
                    height = 32
                },
                --health = {
                --    health_percentage = 100
                --},
                --projectile_emitter = {
                --    projectile_velocity = { x = 200, y = 0 },
                --    projectile_duration = 1, -- secondsm
                --    repeat_frequency = 1, -- seconds
                --    hit_percentage_damage = 10,
                --    friendly = false
                --},
                on_update_script = {
                    [0] = function(entity, delta_time, ellapsed_time)
                        print("Executing BF-109 Lua script!")

                        -- change the position of the the airplane to follow a sine wave movement
                        --local new_x = ellapsed_time * 0.09
                        --local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
                        --set_position(entity, new_x, new_y) -- set the new position
                    end
                }
            }
        }
    }
}

-- Define some useful global variables
map_width = Level.tilemap.num_cols * Level.tilemap.tile_size * Level.tilemap.scale
map_height = Level.tilemap.num_rows * Level.tilemap.tile_size * Level.tilemap.scale
