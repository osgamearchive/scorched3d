----------------------------------------------------
             Apocalypse v3.1 Information
----------------------------------------------------
Apocalypse is a modification for Scorched 3D which adds many new weapons, items, and landscapes
for use with this awesome game.  The mod is the product of work by several people (named in authors.txt).
All original work contained in the mod is copyright.  Legal information can be found in license.txt.


Official Apocalypse Site:  http://apochq.handwired.net
Official Scorched 3D Site:  http://scorched3d.co.uk

Changes Since 3.0
	New Accessories:
		- Flamethrower from Shockmod reduced flame trail lengths though
		- Satellite Strike - projectile marks target, then satellite based lasers assault the area
			
	Other Accessory Changes:
		- New Projectile Model for Mega Mirv/S-Mirv by Shockwave
		- Increased EMP effectiveness against shields slightly and reduced damage to tank
		- Decreased cost of Riot Drill, Wall of Dirt and Ring of Dirt slightly
		- Removed Disco Bunny
		- Other updates to projectile models/texturing
			
	Landscapes:
		- Changes to cities:
			- Retextured many of the existing building, again (thanks Shockwave)

		- New buildings: 
			- bar1

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
landscapesdefn.xml  - Info regarding the shape of the landscapes
landscapesplace.xml - Info regarding placement of the landscape objects
landscapestex.xml   - Defines all the texture layouts that are used by the landscapes
server.xml          - Example Server settings for running an Apocalypse Server
singletarget.xml    - Default target practice game settings
singleeasy.xml      - Default easy difficulty single player game settings
singlenormal.xml    - Default normal difficulty single player game settings
singlehard.xml      - Default hard difficulty single player game settings
singlegames.xml     - Defines menu icons/text for the mod
tankais.xml         - File containing parameters for the ai players
textureset.xml      - Definitions for the textures used in explosion animations

data/accessories/*  - All custom models and their textures used by the mod
data/landscapes/*   - All the custom heightmap and picture files for Apocalypse landscapes
data/textures/*     - All custom weapon icons, explosion textures or landscape textures used by the mod
data/wav/*          - All custom sounds used by the mod
data/windows/*      - GUI windows

Note: Server settings are not required but are provided as an example

----------------------------------------------------
                  Apocalypse Weapons
----------------------------------------------------
Acid Blast      - Explodes releasing some small, earth eating bomblets
Acid Rain       - Non damaging, earth eating rain
Acid Splash     - Explodes on impact and splashes dirt eating bomblets all over the area
Apocalypse      - Warhead that splits a few times in the air realeasing many medium sized nuclear warheads
Baby Napalm     - Weak version of napalm
Bottomless Pit  - Creates a narrow but deep pit at the point of impact
BouncingBetty   - Bomb that bounces and releases multiple explosive warheads
Bunny Bomb      - MIRV with leaping warheads
Chain Lightning - Explodes and releases an aimed arc of electricity with courses through the ground aimed at nearby tanks
Dmsday Device   - Larger, more destructive version of Apocalypse
Earthquake      - Shakes the earth and does damage over a large area from a random location on the map.
EMP Grenade     - Explodes on impact causing damage to opponents without deforming the earth.
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
Instamountain   - Creates a mountain of dirt upon impacting the ground
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
MOAB            - Super-sized Nuke
Napalm Blast    - Explodes on impact and releases many small napalm bomblets
Napalm MIRV     - MIRV with napalm warheads
Napalm S-MIRV   - Spread MIRV with napalm warheads
Quicksand       - Explodes on impact releasing a series of slow-moving, poorly aimed diggers
Ring of Dirt    - Raises multiple dirt walls that form a ring around you
Riot Drill      - 3 heavy riot bombs are shot in rapid succession at the same point
Sat. Strike     - Shoots a projectile which paints the target area.  Space based lasers then assault the target.
Shockwave       - Creates a massive energy wave which ripples outwards without destroying the landscape
Strangelove     - The end game weapon
Suicide Bomb    - Detonates yourself causing a huge, powerful explosion
The Travolta    - Super-sized Funky Bomb
Wall of Dirt    - Raises a small wall of dirt a short distance from your tank in the direction you aim

----------------------------------------------------
                     New Items
----------------------------------------------------
Hv Frc Shield   - Larger, more powerful version of the force shield
Ultra Shield    - Most powerful standard defensive shield

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
	Craters     - Static:  Craterous map with a nice moonlike feel
	Pyramids 2  - Static:  Hand remake of the original Pyramids map
	Desert Mesa - Static:  Southwestern US style map
	Terraces    - Static:  Landscape made up of various height levels
	Steppes     - Static:  Similar to terraces but with a different shape
	Canyon      - Static:  Deep canyon to annihilate eachother in
	City Blocks - Static:  Map with some building like structures
	
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
