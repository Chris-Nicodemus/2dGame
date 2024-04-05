# gameframework2d (GF2D)
a collection of utlitity functions designed to facilitate creating 2D games with SDL2
This project is specifically intended to function as an educational tool for my students taking 2D Game Programming.

FOR FULL FEATURES CHECKOUT THE MASTER BRANCH
Students: Don't do that.  You are not ready.... yet.

The main branch is kept deliberately minimal to allow students to build out systems as they are learning.
Other branches have more complete system: Collisions, Windows, Armatures, Entities, Particles...

# Build Process

Before you can build the example code we are providing for you, you will need to obtain the libraries required
by the source code
 - SDL2
 - SDL2_image
 - SDL2_mixer
 - SDL2_ttf
There are additional sub modules that are needed for this project to work as well, but they can be pulled right from within the project.
Performable from the following steps from the root of the cloned git repository within a terminal. 

Make sure you fetch submodules: `git submodule update --init --recursive`
Go into each submodule's src directory and type:
`make`
`make static`

Once each submodule has been made you can go into the base project src folder anre simply type:
`make`

You should now have a `gf2d` binary within the root of your git repository. Executing this will start your game.

# How to Play
On startup the game will check to see if you have a saved deck of cards, if not, it will generate one for you.

You will be presented with three options at the start of the game, click on one to select an event, a battle, or to rest and heal.

You can press "M" at any time to check your progress through the game.

You may also press "D" at any time to see what your deck is looking like.

# Combat
You start each turn with 3 energy and draw 5 cards. Every card has an associated energy cost, and will require that much energy to play them.

Click on a card to play it. Some cards will require you to select a target, which can be done by clicking on an enemy.

Some cards allow you to gain "Block," which will reduce incoming damage by the amount of block that you have. Keep in mind that you will lose all block after your turn ends.

Some cards mention the keyword "Airborne." You are a transforming mech, which has the ability of flight! Many cards are improved while you are airborne.
Airborne also reduces the next source of damage you recieve by 3, but you also fall to the ground and lose your airborne bonuses.

At the end of your turn, you discard your hand

# Credits

Slay the Spire custom card maker
https://kkouch.itch.io/slay-the-spire-custom-card-maker

Mech-punch by arttukorppu
https://www.deviantart.com/arttukorppu/art/Mech-punch-851701893
Bubble by LeonovichDmitriy on DeviantArt
https://www.deviantart.com/leonovichdmitriy/art/Bubble-559087701

Icon Images from CraftPix
https://craftpix.net/freebies/free-40-loot-icons-pixel-art/?num=1&count=579&sq=icon&pos=6
https://craftpix.net/freebies/free-pixel-art-icons-for-mine-location/?num=4&count=579&sq=icon&pos=7

Stone wall texture by bcmartini77
https://www.deviantart.com/bcmartini77/art/Seamless-Stone-Wall-Texture-524352311

Free City Background Pixel Art from CraftPix
https://craftpix.net/freebies/free-city-backgrounds-pixel-art/

Font
https://www.1001freefonts.com/arkitech.font

<a href="https://www.freepik.com/free-photo/wooden-floor-background_4100933.htm#query=parchment&position=3&from_view=keyword&track=sph&uuid=f6cec357-7b95-458c-b2fc-bf33e41ec647">Image by rawpixel.com</a> on Freepik

https://www.cleanpng.com/png-question-mark-computer-icons-portable-network-grap-7031667/

Button from caftpix
https://craftpix.net/freebies/free-buttons-2d-game-objects/?num=1&count=58&sq=end%20turn%20button&pos=13


Wingsuit by novaillusion
https://www.deviantart.com/novaillusion/art/wingsuit-622539459

HAWK Flight Suit by Richie mason - zbrushtuts
https://ru.pinterest.com/pin/298715387763940674/?amp_client_id=CLIENT_ID%28_%29&mweb_unauth_id=%7B%7Bdefault.session%7D%7D&simplified=true
Airdrop! By Chuk Wojtkiewicz
https://www.artstation.com/artwork/air-drop-6ccdc397-404f-41ea-b4fc-b66203d67f44

Airstrike! By Luca Bottazzi
https://www.artstation.com/artwork/e0lAEY

https://wallhaven.cc/w/j5vq3m

Giant Mech by Met4lhe4d
https://www.deviantart.com/met4lhe4d/art/Giant-Mech-963830952

Portal from Legend of Korra
https://tvtropes.org/pmwiki/pmwiki.php/Main/PillarOfLight

Music by Alex Cristoforetti from Pixabay
Motivational Electronic | Distant
https://pixabay.com/music/beats-motivational-electronic-distant-132919/
Used as game opening

Music by Evgeny Bardyuzha from Pixabay
Password Infinity
https://pixabay.com/music/beats-password-infinity-123276/
Used as general theme

