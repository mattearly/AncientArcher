For this final project in CS4250, I choose to make a small senario from a fighting type first person video game.
The idea was to have the player fight through the level and make it to the end using a Melee and a Ranged attack
as they see fit, featuring a full range of movement and control. Ideally there would be pickups or powerups of
some sort along the way that would change the course of the gameplay. I wanted to do a project like this because
this is often the type of game I enjoy and would like to go about making my own variations.

This was a bit of a stretch for the time allotted, but I tried to make it as far as I could by the time it was
due. While I was able to implement a good portion of these features that I desired, I ended up spending most of
my time abstracting out ways to handle the intended features. Essentially, I was building my own custom Game
Engine, which is now known to me as the Ancient Archer Engine in this early form.

I was able to sift through my Git commits and take a look at the work I had done and the amount of time each
feature ended up taking. I started out the project by first implementing the Shaders. I used a shader class from
JoeyDeVries' tutorial on learnopengl.com. The shader file can be found at:
https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/shader.h
I had to make some modifications to make it work for me but it is mostly intact. This one is nice
because it makes it easy to work with more shaders in the future and you just implement a new
shader object and open new files. It has nice functions for setting the uniform variables as well.

The tutorials on learnopengl.com are decent for getting started, but many things can be difficult to figure
out at first. I had to do a lot of research on my own to figure out things that were not explained well but
luckily there are quite a few people talking about it so it wasn't too hard. I left a few comments of my own
to help others in the same places I got stuck.

From my last project, the big thing I felt like I was missing to make it look like a real game (other than
lighting) was textures, so I set those up next. This turned out to be a rather long process. I ended up using
stb_image.h to load in pictures, which can be found here:
https://github.com/nothings/stb/blob/master/stb_image.h

I'm using glad.c (in place of glew.h) to gather all the opengl functionality. 
I'm using glm libary to help handle math (it replicates the shader math and variables for use in C code)
I'm using glfw as the window manager, it handles first person with no mouse much better than glut, as well
as many other things.

I could have used SDL_image as insead, but stb_image.h seems more lightweight and suits the purposes here. One
thing about textures I had to learn by digging through kronos.com documentation is that OpenGL in general
works better with certain image specifications (width divisable by 4, has an alpha channel, is a .png). I
worked on my own custom 'TextureBank' class to handle loading in textures and setting one at a time as the
active. This took longer than expected to work out the kinks (2 days), but has already paid off in the clarity 
if brings the project and the sheer number of lines of code it cut. I learned a lot by putting this together.

I should also mention that porting this from Linux over the Windows with access to the latest OpenGL tools
that most developers take advantage of, took me several days but is now well worth it as I plan on using this
going forward.  Linux is nice for programming in general, but the Graphics Drivers where beginning to cause
me trouble when trying to go to a more modern version of OpenGL, hence the switch to Windows. This was also
a great opportunity for me to use Visual Studio for once, as it is a common industry standard peice of software
that I should be familiar with.

I added sounds back in. I noticed there are a few small issues with the way it is done (all static, no class).
It doesn't cause any issues in a small project such as this, but in the future I do play on making vast
improvements to the sound system. I'll probably use FMOD in the future as that seems to be what most video games
with impressive sound use.

I tried adding Lighting, but things went haywire and I lost a solid day with little to no progress. I benched the
 idea in order to be able to continue constructing my game. I play to add lighting about the same time I abstract
out primative's and model loading, that way they can tie in together as intended. Something for the future...

I spent a great deal of time tweaking the controls, movement, and feel. I made the jump feel as good as it could
before adding in realistic gravity (another thing for the future, right now things just fall at a fixed rate). 
I added in a few basic player variables (Leg Power and Attack Speed). The leg power affects how fast you walk/run
and also how fast you rise from your jump and how high you jump. Attack Speed effects how fast you can attack.
These stats can be buffed/debuffed throughout the game. They do have a Cap so they don't scale out of control.
They are also put through a custom algorithm in order to scale them more realistically while having 1 to 100
as the viable scores.

This might be a good time to talk about controls:
WASD -> move directions
hold LEFT SHIFT -> Run
Spacebar -> Jump
LeftClick -> Attack
1 -> switch to melee (sword)
2 -> switch to ranged (bow)
0 -> sheath weapons

the move, run, jump and switched are carefully managed to not allow weird things. You can't jump again until
you've: a) released the spacebar, b) landed back on the ground. You can't Run when you are moving Backwards.
Cases such as those were handled, and a few others. Cases I still need to adjust: you can move through the air
left and right in what would be impossible physics - I need to lock the player's angel until they land again so
they can't 'snake' throught air or make a U shaped 180 like an airplane. Most people wont notice this, and some
popular games have this control unfixed (I noticed 7 Days to Die has this problem, for example).

I added in some basic pickups to improve stats. They are peppered around the level randomly and feature 
collison for pickup. Once they are all picked up, they respawn. This is mostly to keep the player busy and demo
what the stats feel like at different points until maxxed.

I added in some textures and primative drawing for the player's weapons. They didn't come out as well as I had
hoped and need some more work but they are pretty obvious once the sound effect plays along with them.

I had to add in timers between attacks to work with the Attack Speed.  The more Attack Speed you get,
the faster you can Attack and this should be noticable as pickups are collected.  I tweaked the movement footstep
sounds to go twice as fast when you run, so it feels more realistic.

There were a lot of humps to overcome while programming this, and I'll try to cover some of them during my
presentation.


