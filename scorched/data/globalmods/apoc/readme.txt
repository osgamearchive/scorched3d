----------------------------------------------------
             Apocalypse v2.0 Information
----------------------------------------------------
Apocalypse is a modification for Scorched 3D which adds many new weapons, items, and landscapes
for use with this awesome game.

Official Apocalypse Site:  http://apochq.handwired.net
Official Scorched 3D Site:  http://scorched3d.co.uk

Changes Since 1.6
	New Accessories:
		- Bottomless Pit - Creates a narrow but deep pit at the point of impact
		- Earthquake - shakes the earth and does damage over a large area
		- EMP Grenade - Bounces once and explodes causing damage to opponents without deforming the earth.
		- Hard Hat - Replacement for the old style heavy mag
		- Hornet Swarm - Releases multiple Hornet's Nests
		- Gas Grenade - Lands, bounces and releases a cloud of harmful gas.  Custom model by ShoCkwaVe
		- Grenade - Bounces once and does a fairly small explosion.
		- Lightning Strike - Explodes at the apex of the projectiles arc creating a small cloud and lightning bolt
		- Lightning Storm - Explodes at the apex of the projectile's arc creating several lightning bolts over a few seconds
		- Mega EMP - Larger version of the EMP grenade
		- Mini Hard Hat - Replacement for the old style mag deflect shield
		- Ring of Dirt - Raises multiple walls of dirt forming a ring around your tank
		- Riot Drill - 3 heavy riot bombs are shot in rapid succession at the same point
		- Wall of Dirt - Raises a wall of dirt a short distance from your tank in the direction you aim

	Other Accessory Changes:
		- Acidic Napalm - Secondary, random splash when the bomblets impact, new texture
		- Acid Splash - Bounces once releasing additional random bomblets.  New model/texture by ShoCkwaVe
		- Chain Lightning - Travels in straight lines from point to point instead of bouncing
		- Fire Worms - More projectiles/cost, diggers are less accurate and slower
		- Fork Lightning - Same new effect as chain lightning
		- Hornet's Nest - Releases all the hornet's over a period of time.  New model by ShoCkwaVe
		- Mega MIRVs - Larger explosions but less of them
		- Meteors:
			- New delayed smolder before the meteor appears
			- More flaming earth but now flys in random directions
		- Quicksand - Stays below ground, diggers are slower moving
		- Shockwave - Now an energy wave that ripples outward instead of bouncing
		- Shockwave m2 - added multicolor tags and renamed to the Disco Bunny
		- Suicide Bomb - Short Delay before the explosion goes off
		
	Landscapes:
		- Apoc City - New landscape that comes complete with a sprawling city to destroy
		- Arizona Islands - New island map by ShoCkwaVe
		- Volcano - A single island with a volcano in the middle
		- Maze - raised the entire map up some, lowered wall height and widened the lanes to make the map easier.  Also tweaked the map so napalm works better
		- Cities added to some other maps
		- Moon maps have a new texture set and rocks added to the landscape
	
	Other:
		- New elemental AI types based off of the new Cyborg AI type
		- New default single player settings for the mod
		- Mod Updated For Compatibility With Scorched3D version 38
		- Most of the custom weapons have their own icons
		- Several weapons commented out or removed for various reasons :P
			- Acidic Napalm, Baby Funk, Fungal Bloom, Large Missile, Napalm Blast, Napalm Splash, Roller MIRV, Roller S-MIRV, Sandfish, Sandhog Blast, Shotgun, Terraform, X-MIRV

----------------------------------------------------
                     Files
----------------------------------------------------
authors.txt			- Contains the names of all contributors to the Apocalypse Mod, and what they contributed
license.txt			- License for the mod and related materials
readme.txt			- This file
readme-nexus.txt	- Readme file that comes with Nexus6's maps

data/				- Directory containing all data files and other media used by the mod
accessories.xml		- File containing all the item/weapons for the game
ainames.txt			- Contains the custom names for the bots (all names of countries for Apocalypse)
landscapes.xml		- File containing basic landscape data
landscapesdefn.xml	- Detailed landscape data
landscapestex.xml	- Defines all the texture (and object) layouts that are used by the landscapes
server.xml			- Example Server settings for running an Apocalypse Server
singletarget.xml	- Default target practice game settings
singleeasy.xml		- Default easy difficulty single player game settings
singlenormal.xml	- Default normal difficulty single player game settings
singlehard.xml		- Default hard difficulty single player game settings
singlegames.xml		- Defines menu icons/text for the mod
tankais.xml			- File containing parameters for the ai players
textureset.xml		- Definitions for the textures used in explosion animations

data/accessories/*	- All custom models and their textures used by the mod
data/landscapes/*	- All the custom heightmap and picture files for Apocalypse landscapes
data/textures/*		- All custom weapon icons, explosion textures or landscape textures used by the mod
data/wav/*			- All custom sounds used by the mod

Note: Server settings are not required but are provided as an example

----------------------------------------------------
                  Apocalypse Weapons
----------------------------------------------------
Note: Any weapon with an "x" in front is included in the mod but has been commented out in the official release
If you would like to add any of these weapons to your server, simply remove the <!-- and --> symbols surrounding them

Acid Blast	- Explodes releasing some small, earth eating bomblets
Acid Rain	- Non damaging, earth eating rain
Acid Splash	- Explodes on impact and splashes dirt eating bomblets all over the area
Acidic Napalm	x Explodes on impact releasing dirt eating/burning bomblets 
Apocalypse	- Warhead that splits a few times in the air realeasing many medium sized nuclear warheads
Atlantis Bomb	- Explodes in the air and releases many dirt eating bombs that destroy most of the map without doing physical damage to tanks
Baby Funk	x Small Funky Bomb
Baby Napalm	- Free, weak version of napalm
Bottomless Pit  - Creates a narrow but deep pit at the point of impact
BouncingBetty	- Bomb that bounces and releases multiple explosive warheads
Bunny Bomb	- MIRV with leaping warheads
Chain Lightning - Explodes and releases an aimed arc of electricity with courses through the ground aimed at nearby tanks
Digger Blast	x Explodes on contact and releases many baby diggers
Dirt Wall       - Raises a wall of dirt a short distance from your tank in the direction you aim
Disco Bunny	- Releases an initial salvo of aimed warheads which bounce outwards creating more warheads as they bounce
Dmsday Device	- Larger, more destructive version of Apocalypse
Earthquake      - shakes the earth and does damage over a large area
EMP Grenade     - Explodes on impact causing damage to opponents without deforming the earth.
Fatman		- In between a nuke and a MOAB
Fire Ants	- MIRV that releases multiple rolling, explosive napalm warheads
Fire Worms	- Creates a series of slow-moving, explosive napalm diggers
Fork Lightning	- Larger version of chain lightning which splits up into several arcs as it progresses
Fungal Bloom	x Bomb that explodes and spreads dirt everywhere
Gas Grenade     - Lands, bounces and releases a cloud of harmful gas.  Custom model by ShoCkwaVe
Haymaker	- MIRV with Bouncing Betty warheads
Hellstorm	- Firey/explosive rain
Hornet's Nest	- Creates an angry cloud of hornets which fly out and attack nearby tanks
Hornet Swarm    - Releases multiple Hornet's Nests
Instamountain	- Creates a mountain of dirt upon impacting the ground
Lightning Bolt  - Explodes at the apex of the projectile's arc creating a small cloud and lightning bolt
Lightning Storm - Explodes at the apex of the projectile's arc creating several lightning bolts over a few seconds
Mega EMP        - Larger, more powerful version of the EMP grenade
Mega Meteor	- Larger, more powerful version of meteor
Mega MIRV	- Larger, more powerful version of the standard MIRV
Mega S-MIRV	- Larger, more powerful version of the spread MIRV
Meteor		- Meteor appears at the peak of flight and lands scattering flaming earth
Meteor Shower	- Creates many meteors that rain down upon everyone
MOAB		- Super-sized Nuke
Napalm Blast	x Explodes on impact and releases many small napalm bomblets
Napalm MIRV	- MIRV with napalm warheads
Napalm S-MIRV	- Spread MIRV with napalm warheads
Quicksand	- Explodes on impact releasing a series of slow-moving, poorly aimed diggers
Riot Drill      - 3 heavy riot bombs are shot in rapid succession at the same point
Roller MIRV	x MIRV with rolling warheads
Roller S-MIRV	x Rolling Spread MIRV
Sandfish	x Explodes on impact releasing several leaping sandhogs
Sandhog Blast	x Explodes on contact and releases many baby sandhogs
Shockwave	- Creates a massive energy wave which ripples outwards without destroying the landscape
Strangelove	- The end game weapon
Suicide Bomb	- Detonates yourself causing a huge, powerful explosion
Terraform	x Raises dirt over a huge area when fired high into the air
The Travolta	- Super-sized Funky Bomb

----------------------------------------------------
                     New Items
----------------------------------------------------
Hv Frc Shield   - Larger, more powerful version of the force shield
Ultra Shield    - Most powerful standard defensive shield

----------------------------------------------------
                  Apocalypse Maps
----------------------------------------------------
Maps By Bobirov:
	BigHill		- Random:  Generally a single very large hill
	Mountains	- Random:  Very mountainous map
	Ring		- Random:  Small valley or pond always in the middle
	Apoc City	- Static:  Island map that comes complete with a sprawling city to destroy
	World Map	- Static:  Map of the entire planet
	USA		- Static:  Map of the continental U.S.
	Bullseye	- Static:  Round medium-height map with a few elevation levels, city-enabled
	Mesa City	- Static:  Square map with a high, flat surface, city-enabled
	Pillars		- Static:  Series of steep outcroppings of various heights
	Colloseum	- Static:  Round "arena-like" map
	Ruffles		- Static:  Map that looks pretty much like a ruffles potato chip
	Maze		- Static:  A nice maze with players spawning only inside on the ground level
	Volcano		- Static:  An volcanic tropical island.

Maps By Nexus6:
	Castle		- Static:  Sand castle looking map
	Moon		- Static:  Nice hilly, moon like map
	Pyramids	- Static:  Moderate height map with some nice pyramids to blow up
	Weathered	- Static:  Another nice hilly map
	Hex World	- Static:  Cool map made up of hexagonal shapes of varying heights
	Two Towers  	- Static:  Nice map with two large round towers
	Craters		- Static:  Craterous map with a nice moonlike feel
	Pyramids 2	- Static:  Hand remake of the original Pyramids map
	Desert Mesa 	- Static:  Southwestern US style map
	Terraces    	- Static:  Landscape made up of various height levels
	Steppes		- Static:  Similar to terraces but with a different shape
	Canyon		- Static:  Deep canyon to annihilate eachother in
	City Blocks	- Static:  Map with some building like structures
	
Maps By ShoCkwaVe:
	ArizonaIslands	- Static:  Series of islands that resemble rock formations in the western U.S.

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
