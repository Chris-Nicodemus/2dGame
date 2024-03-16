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
