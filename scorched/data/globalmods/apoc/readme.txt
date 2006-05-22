----------------------------------------------------
             Apocalypse v4.0 Information
----------------------------------------------------
Apocalypse is a modification for Scorched 3D which adds many new weapons, items, and landscapes
for use with this awesome game.  The mod is the product of work by several people (named in authors.txt).
All original work contained in the mod is copyright.  Legal information can be found in license.txt.


Official Apocalypse Site:  http://apochq.handwired.net
Official Scorched 3D Site:  http://scorched3d.co.uk

Changes Since 3.1
    New Accessories:
        - Tsar Bomba - the largest nuclear bomb ever made courtesy of the Soviets.
            - Basically takes place of the old style MOAB behaviour, a big explosion & crater
            - Slightly smaller explosion crater than old MOAB
            - Invisible radioactive fallout does additional damage to surrounding tanks
        - Blink Device - a prototype near-instant teleporter
        	- However, the new technology is unstable and frequently collapses in a violent explosion
        - Transporter - shoots a marker projectile and then transports the user to that location
        - X Tracer - Shoots a spread of tracers at 2.5 degree increments
            - Inspired by Brain Damage's 'Super Tracer +' :)
        - Nitro - faster moving fuel that costs more
            
    Weapons Removed or Made Unpurchaseable due to lack of use, redundancy or ineffectiveness
        - Bouncing Betty
        - Instamountain
        - Ring Of Dirt
        - Landscaper
        - Fire Ants
        - Dirt Charge
        - Heavy Roller
        - Heavy Sandhog
        - Heavy Digger
        - Tracer
        - Hot Napalm
        
    Other Accessory Changes:
        - Sat. Strike:
            - Tweaked aiming methods some more, works better against shields
            - Slowed the overall pace of it down substantially and reduced amount of aimed projectiles
            - Increased price from $20,000 to $50,000
            - Reduced armslevel from 2 to 1
        - M2 Machinegun:
            - Increased size of the bullets by 50%, they're still small but more visible
            - Reduced damage the bullets do during shield collisions to 33% of shield value
            - Reduced damage of the shells from 0.4 to 0.2 (total damage from 200 to 100)
            - Now has a somewhat random spread pattern instead of hard-coded
        - Shotgun:
            - Increased amount of projectiles from 5 to 6
            - Decreased damage during shield collisions from 50% to 20% of shield value
            - Decreased damage done per pellet from 0.25 to 0.2 (total damage from 125 to 120)
            - Pellets now have a somewhat random spread pattern
        - Suicide Bomb:
            - Reduced the size of the crater (but not damage range) considerably
            - Added flaming debris
            - Reduced cost from $16,000 to $12,000
            - Added some more small explosions to hopefully make sure it kills the user even when shielded
        - MOAB:
            - Reduced crater size to between missile and baby nuke
            - Air blast does some additional damage outside the crater
            - Reduced cost to $2,000 for 2
            - This brings the weapon a little more in line with the real thing
        - Shockwave:
            - Removed reliance on WeaponAimedOver, now spreads at 22.5 degree angles
            - Added a few more sparks after the main shockwave begins for visual effect
            - Decreased damage just barely
        - Lightning Storm:
            - Added lightningbolt.wav sounds back in
            - Increased cost from $25,000 to $30,000
            - Slowed down the pace by a few seconds to make it easier to watch
        - Hornet Nest/Swarm:
            - Increased cost of Hornet Nest to $40,000
            - Decreased armslevel of Hornet Nest from 2 to 1
            - Hornets come out 1 and 2 at a time instead of in big groups
        - Rollers:
            - Increased cost of Baby Roller to $4,000 for 2
            - Decreased armslevel of Baby Roller from 7 to 6
            - Rollers now hurt shields slightly when they collide
            - Rollers are dropped from slightly above the collision to help prevent shield popping
        - Flame Thrower:
            - Added random spread pattern
            - Decreased power by about half
            - Decreased cost to $4,000 for 2
        - Napalm Weapons
        	- Increased cost to $6,000 for 2
        	- Decreased power of Napalm slightly
        	- Decreased power of Napalm Mirv/S-Mirv slightly
        	- Streams come out one at a time in Napalm Blast
        	- Napalm Blast leaves some napalm at the collision point as well
        - Frag Grenade:
            - Increased velocity of the shrapnel
            - Added collision timer to the shrapnel
            - Decreased shieldhurtfactor of shrapnel pellets from 100% to 10%
            - Increased number of shrapnel pellets from 8 to 9
            - Reduced damage per shrapnel pellet from 0.6 to 0.4
            - Added short flame trail to shrapnel and reduced smoke trail length
        - Acid Weapons:
        	- New acid behaviour (gradual earth erosion with green napalm burn effect)
        	- Reduced shieldhurtfactor on all acid weapons to around 25% per drop
        	- Increased cost of Acid Splash to $10,000
        	- Increased cost of Acid Blast to $3,000 for 2
        - Chain/Fork Lightning
        	- Reduced power/accuracy and price
        - Shields:
            - Decreased radius of Heavy Shield slightly (between regular and ultra size)
            - Increased Mag Deflect's <deflectpower> from 50% to 75%
            - Reworked shield <penetration> values.
                - Regular shields offer most raw damage resistance, then mags, then force shields.
            - Flattened out base projectile collision damage for all regular shields at 10 pts
            - Flattened out base projectile collision damage for all force shields at 15 pts
            - All shields still have 100 base pts of health
            - Updated descriptions to include some info about the total amount of damage they can absorb
                and how much damage they take when being hit by projectiles by default
        - Nuke, Fatman and Tsar Bomba do additional collision damage to shields due to large mass (133-200%)
        - Reduced shield collision damage on diggers to decrease their usefulness as shield busters
        - Apoc/Dday/Strangelove now use a fixed spread (not affected by projectile's speed at shot peak)
        - Added clouds to Apoc/Dday/Strangelove for visual effect
        - Added some lightning bolts in the clouds in Strangelove
        - Added another projectile to Apoc/Dday/Strangelove waves
        - Added second wave of projectiles to Apocalypse & Dmsday Device
        - Added some extra aimed projectiles to Strangelove after ground collision like it used to be pre v3.0
        - Added a small sound for hornets when they sting to replace the flare sound.
        - Added puff of smoke to Instamountain
        - Added descriptions to the damaging building accessories
        - Increased damage from lasers some
        - Increased accuracy of Sandhogs slightly (they are still less powerful than official ones tho)
        - New projectile model for MOAB by Bobirov
        - Made Gas Stations a little bit more damaging all around
        - Slightly increased power of Small Meteor explosion (5 percent)
        - Reduced interval between drops and reduced strength a tad in Hell Storm
        - Reduced max physical damage of EMP's from 50 to 25 (shield damage went up from 75 to 100)
        - Decreased size of Dirt Clod a bit to reduce its use as a bunker builder
        - MIRV and Mega MIRV now use more reliable redirect code
        - Set Wall Of Dirt maximum to 3 per round
        - Set Ring Of Dirt maximum to 1 per round
        - Fixed oversight in EMP grenade causing it to do more damage to shields than EMP missile
        - Tracers now do no damage to shields when they hit
        - Renamed Suicide Bomb to Self Destruct
        - Reduced cost of Haymaker to $30,000
        
    Landscapes:
    	- New Nuclear Power Plant Building
    		- Explodes and releases massive amounts of fallout that travels down wind
    	- Factories now drop 1-3 gas/acid weapons which can be shot and picked up
    	- Stores now drop 1-3 various small weapons, batteries, beer (10 health) or $1,000 bundles
    	- Gas stations drop 1-3 fuel cans (10 fuel) or $1,000 bundles
    	- Bars drop 1-3 beers (10 health), shotguns or $1,000 bundles
    	- New hospital by Shockwave that drops 1-3 medkits (25 health)
    	- New port by Shockwave
    	- Houses can no longer keep setting themselves on fire
    	- New 737 jumbo jet boid by Shockwave on cities
    	- New cessna boid by Shockwave on cities
    	- New medical chopper boid by Shockwave on cities
    	- Added some flying hornets to the desert maps
    	- Added several groundmaps to city buildings (roads, farmland, cars, etc)
    	- Moon maps:
    		- Added stars
	        - Removed clouds as best I could (there is no atmosphere on the moon of course.. :P)
	        - Adjusted light levels a bit
	        - Added view of the Earth
        - Tweaked surround texture for desert maps some so its a little less repetitive looking
        - Reduced height of Church model by 25%
        - Removed City Blocks map
        - Replaced f18 model on city maps with f16
        
    Other:
        - Added Sniper AI type that uses mostly lasers and guns.
        - Added a "Hard Target" AI type that does not shoot but will use defenses, good for testing
        - Various other small changes and fixes
        
Notes About Installation:
The Apocalypse Mod comes with Scorched 3D and, as a result, you should not need to install anything.  However, 
in the event that you are re-installing the mod because it has become corrupt, it should be installed into the
directory you installed Scorched3D to, under /data/globalmods/.  It wouldn't be a bad idea to delete what is in
the Apoc directory there and in your ~/.scorched3d/mods/apoc/ folder before re-installing as well.

Updates for the mod can typically be obtained on the official Apocalypse servers at ApocHQ, or by going to 
http://apochq.handwired.net.  

        
----------------------------------------------------
                     Files
----------------------------------------------------
authors.txt         - Contains the names of all contributors to the Apocalypse Mod, and what they contributed
license.txt         - License for the mod and related materials
readme.txt          - This file
readme-nexus.txt    - Readme file that comes with Nexus6's maps

data/               - Directory containing all data files and other media used by the mod
accessories.xml     - File containing all the item/weapons for the game
ainames.txt         - Contains the custom names for the bots (all names of countries for Apocalypse)
landscapes.xml      - File containing basic landscape data
server.xml          - Example Server settings for running an Apocalypse Server
singletarget.xml    - Default target practice game settings
singleeasy.xml      - Default easy difficulty single player game settings
singlenormal.xml    - Default normal difficulty single player game settings
singlehard.xml      - Default hard difficulty single player game settings
singlegames.xml     - Defines menu icons/text for the mod
tankais.xml         - File containing parameters for the ai players
textureset.xml      - Definitions for the textures used in explosion animations

data/accessories/*  - Custom models and their textures used by the mod
data/landscapes/*   - All the custom heightmaps, pictures and xml files for Apocalypse landscapes
    ambientsound*.xml - Stores ambient sound groupings
    defn*.xml       - Info regarding the shape of a landscape
    place*.xml      - Info regarding placement of the landscape objects
    tex*.xml        - Defines all the texture layouts that are used by the landscapes
data/meshes/*   - Boid models used by the mod
data/textures/* - All custom weapon icons, explosion textures or landscape textures used by the mod
data/wav/*      - All custom sounds used by the mod
data/windows/*  - GUI windows

Note: Server settings are not required but are provided as an example

----------------------------------------------------
                  Apocalypse Weapons
----------------------------------------------------
Acid Blast      - Explodes releasing some small, earth eating bomblets
Acid Rain       - Non damaging, earth eating rain
Acid Splash     - Explodes on impact and splashes dirt eating bomblets all over the area
Apocalypse      - Warhead that splits a few times in the air realeasing many medium sized nuclear warheads
Baby Napalm     - Weak version of napalm
Blink Device    - Prototype, near-instant teleporter.  Portal created is unstable.
Bottomless Pit  - Creates a narrow but deep pit at the point of impact
BouncingBetty   - Bomb that bounces and releases multiple explosive warheads
Bunny Bomb      - MIRV with leaping warheads
Chain Lightning - Explodes and releases an aimed arc of electricity with courses through the ground aimed at nearby tanks
Dmsday Device   - Larger, more destructive version of Apocalypse
Earthquake      - Shakes the earth and does damage over a large area from a random location on the map.
EMP Grenade     - Grenade that does heavy damage to shields and light damage to tanks
EMP Missile     - Missile with a larger EMP effect than the grenade
Fatman          - In between a nuke and a MOAB
Fire Ants       - MIRV that releases multiple rolling, explosive napalm warheads
Fire Worms      - Creates a series of slow-moving, explosive napalm diggers
Flame Thrower   - Shoots a stream of napalm at the target
Fork Lightning  - Larger version of chain lightning which splits up into several arcs as it progresses
Gas Grenade     - Lands, bounces and releases a cloud of harmful gas.  Custom model by ShoCkwaVe
Haymaker        - MIRV with Bouncing Betty warheads
Hellstorm       - Firey/explosive rain
Hornet's Nest   - Creates an angry cloud of hornets which fly out and attack nearby tanks
Hornet Swarm    - Releases multiple Hornet's Nests
Laser Cannon    - Fires 3 regular laser shots at slightly different angles
Lightning Bolt  - Fires from a random location creating a small cloud and lightning bolt
Lightning Storm - Fires from a random location creating several lightning bolts over a few seconds
M2 Machinegun   - Shoots five 50 caliber machinegun bullets in rapid succession.
Mega EMP        - Larger, more powerful version of the EMP grenade
Mega Meteor     - Larger, more powerful version of meteor
Mega MIRV       - Larger, more powerful version of the standard MIRV
Mega S-MIRV     - Larger, more powerful version of the spread MIRV
Meteor          - Meteor appears at the peak of flight and lands scattering flaming earth
Meteor Shower   - Fires from a random location and creates many meteors that rain down upon everyone
MOAB            - Large conventional explosive bomb
Napalm Blast    - Explodes on impact and releases many small napalm bomblets
Napalm MIRV     - MIRV with napalm warheads
Napalm S-MIRV   - Spread MIRV with napalm warheads
Quicksand       - Explodes on impact releasing a series of slow-moving, poorly aimed diggers
Riot Drill      - 3 heavy riot bombs are shot in rapid succession at the same point
Sat. Strike     - Shoots a projectile which paints the target area.  Space based lasers then assault the target.
Shotgun         - Shoots 6 pellets in a random spread with limited range/line of sight
Shockwave       - Creates a massive energy wave which ripples outwards without destroying the landscape
Strangelove     - The end game weapon, scaled up dmsday device
Suicide Bomb    - Detonates yourself causing a huge, powerful explosion
The Travolta    - Super-sized Funky Bomb
Transporter     - Transports the user to the location of a marker projectile
Tsar Bomba      - Largest single nuclear explosive bomb ever made
Wall of Dirt    - Raises a small wall of dirt a short distance from your tank in the direction you aim
X Tracer        - Shoots a spread of tracers at 2.5 degree increments

----------------------------------------------------
                     New Items
----------------------------------------------------
Hv Frc Shield   - Larger, more powerful version of the force shield
Ultra Shield    - Most powerful standard defensive shield
Heavy Mag       - Larger version of the mag deflect

----------------------------------------------------
                  Apocalypse Maps
----------------------------------------------------
Maps By Bobirov:
    BigHill     - Random:  Generally a single very large hill
    Mountains   - Random:  Very mountainous map
    Ring        - Random:  Small valley or pond always in the middle
    Apoc City   - Static:  Island map that comes complete with a sprawling city to destroy
    World Map   - Static:  Map of the entire planet
    USA         - Static:  Map of the continental U.S.
    Bullseye    - Static:  Round medium-height map with a few elevation levels, city-enabled
    Mesa City   - Static:  Square map with a high, flat surface, city-enabled
    Pillars     - Static:  Series of steep outcroppings of various heights
    Colloseum   - Static:  Round "arena-like" map
    Ruffles     - Static:  Map that looks pretty much like a ruffles potato chip
    Maze        - Static:  A nice maze with players spawning only inside on the ground level
    Volcano     - Static:  An volcanic tropical island.

Maps By Nexus6:
    Castle      - Static:  Sand castle looking map
    Moon        - Static:  Nice hilly, moon like map
    Pyramids    - Static:  Moderate height map with some nice pyramids to blow up
    Weathered   - Static:  Another nice hilly map
    Hex World   - Static:  Cool map made up of hexagonal shapes of varying heights
    Two Towers  - Static:  Nice map with two large round towers
    Craters     - Static:  Craterous moon map with a nice view of the earth
    Pyramids 2  - Static:  Hand remake of the original Pyramids map
    Desert Mesa - Static:  Southwestern US style map
    Terraces    - Static:  Landscape made up of various height levels
    Steppes     - Static:  Similar to terraces but with a different shape
    Canyon      - Static:  Deep canyon to annihilate eachother in
    
Maps By ShoCkwaVe:
    ArizonaIslands  - Static:  Series of islands that resemble rock formations in the western U.S.

----------------------------------------------------
                     Legal Stuff
----------------------------------------------------
Here is the human readable version of the license. If you are a lawyer or
just like reading legal documents, take a look at the contents of the
LICENSE file.

Attribution-ShareAlike 2.0

You are free:

    * to copy, distribute, display, and perform the work
    * to make derivative works
    * to make commercial use of the work

Under the following conditions:
    
Attribution. You must give the original author credit.
    
Share Alike. If you alter, transform, or build upon this work, you may
distribute the resulting work only under a license identical to this one. 

    * For any reuse or distribution, you must make clear to others the license terms of this work.
    * Any of these conditions can be waived if you get permission from the copyright holder.

Your fair use and other rights are in no way affected by the above.

This is a human-readable summary of the Legal Code (the full license).
