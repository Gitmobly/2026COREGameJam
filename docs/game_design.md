(meowgoo writing)

Supposedly the theme of the jam is "Actions have consequences",
but as of now we barely have any actions, and definitely no
consequences.

We also have the basic game loop of being a circle that shoots
other circles. I'm seeing someone has been planning on adding
abilities unlockable after death, but that's not remotely finished

So I'm thinking of the most basic way to implement the theme -
the action of killing an enemy has some consequence. More
specifically, I kinda see this as a bullet hell game. So imagine
the following (just a concept for demonstration):

We enter the room, there are 5 enemies. After you kill one enemy
it turns into an indestructible turret that shoots bullets (at a
relatively slow speed, as to not overwhelm you). You also lose your
ability to inflict damage for 5 seconds (or, alternatively, we
spawn a goal that you have to catch by colliding with it, that
grants the ability to inflict damage back). After all enemies have
been killed (even if there are active turrets), the exit opens.

You also have 3 abilities (LMB, RMB and spacebar), although at
the beginning you only have basic shooting on LMB. Periodically you
will be offered to replace one of your abilities of your choosing
with a different one. To stimulate this, we can gradually scale
enemies' stats, and scale offered abilities accordingly (this
feels kinda lacking? idk very much up to discussion)

As such we have a (supposedly) cool dynamic where the gameloop
alternates between trying to kill an enemy, and trying to avoid
enemies+bullets. I think to prevent cheesing we might want to
restore the health of all living enemies each time one is killed

So to boil it down, we have to come up with (in order of importance):

1. A bunch of consequences for killing enemies
    - bullet hell stuff
    - messing with movement/input?
    - somehow affect all remaining enemies
2. A bunch of abilities for the player
    - I kinda want movement-based stuff, but idk if it fits
3. A bunch of enemies
    - for more generally interesting gameplay

Apart from implementing these, we have to additionally implement:

1. Losing ability to inflict damage for some time after killing
2. Ability to choose an ability

***

Playing against the sniper seems to be quite fun, you have to really
aggressively press it against the wall, all while dodging everything
else.

It's not that rare to be quickly bursted down. Sometimes it makes
sense - you have to pay attention to where the enemies are when
they go berserk. But sometimes there's just too much happening
and it feels unfair. I'm thinking of an ability (unlockable or
permanent?) to push away all enemies in some radius, with like 10s
cooldown. Other defensive options would be nice too. Also sometimes
berserk monsters can just push you into a corner and you cant do
anything

Also on the last day of my week I figured out that you can hold LMB
to continuously shoot. This doesn't make it *that* much easier, but
makes me think about *some* ammo management - maybe you can have
a heavy weapon that does a fuckton of damage which needs reloading,
and something lighter without ammo? Potentially making the revolver
crosshair relevant agian?

Also, really important - when playing I kinda feel the game fantasy
of aggressively mowing through enemies while having to mostly dodge
bullets. If we make getting bursted down less often, this could become
even more visceral
