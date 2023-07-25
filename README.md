# UE-78453 Repro and Workarounds
This project contains a small sample for demonstrating Unreal Engine bug
[UE-78453](https://issues.unrealengine.com/issue/UE-78453) and several
workarounds for the bug when using the Gameplay Ability System.

## The Bug

This bug happens when all of the following conditions are true:
1. This is a multiplayer game.
2. There are multiple character pawns among which the player can switch control
   at any time; OR
   the player switches between controlling a floating camera and controlling a
   character pawn.
3. A character has been granted an ability.
4. The ability is initiated from the server side.

When these conditions are true, there is a difference in behavior between what
a client sees and what the server and other clients see. 

## An Example

For example, consider the following setup:
1. There is 1 client and 1 listen server.
2. `Player #1` (`Client #1`) can control `Character A` and `Character B`. 
   Initially, `Client #1` is controlling `A`.
3. `Player #2` (the `Listen Server Client`) can control `Character X` and
   `Character Y`. Initially, the `Listen Server Client` is controlling `X`.
4. All characters have a server-initiated "Run in place" gameplay ability that
   plays a montage.
5. `Player #1` switches to possessing/controlling `Character B` but crosses a
   trigger volume that initiates the "Run in place" action on the server on
   `Character A`.
5. The `Listen Server Client` switches to possessing/controlling `Character Y`
   but crosses a trigger volume that initiates the "Run in place" action on the
   server on `Character X`.

In the setup above:
- `Player #2` will observe that `Character A` is playing the montage, but
  `Player #1` _won't_ see any montage play, even though `Character A` "belongs"
  to `Player #1` and `Player #1` had invoked the action that played the montage
  in the first place.
- Both `Player #1` and `Player #2` will observe that `Character X` is playing
  the montage. This is because `Player #2` is the Listen Server Client, and
  `Client #1` knows that `Character X` is not local to `Client #1`.
  

## Reproducing the Bug with This Project

### Steps

1. Clone this project locally.
2. Check out the [with-bug](https://github.com/OpenPF2/UE-78453-repro/commit/with-bug) tag.
3. Compile the project.
4. Open the project in UE 5.1+.
5. Launch the project with 2 players in "Listen Server" mode.  
6. Click into a "Client" window.
7. Move the character pawn up the ramp until they fall off the other side and 
   start playing a "running in place" animation.
8. Press the `TAB` key to switch which character you are controlling.
9. Move the second character pawn up the ramp until they fall off. 

### Actual Behavior
<img src="./Docs/Images/UE-78453-BugRepro.gif" title="A recording of the difference in behavior between the server and the client." alt="On the server, both characters animate. On the client, only the currently possessed character animates, even though both characters are being instructed to play a montage by the server.">

1. Both the character pawn you were previously controlling and the character 
   pawn you are currently controlling animate in the "Listen Server" window.
2. Only the character pawn you are controlling animates in the "Client" window.

### Expected Behavior (without the bug)
Both the character pawn you were previously controlling and the character
pawn you are currently controlling should animate in both the "Client" and
"Server" windows since the server executes a gameplay ability on both pawns
when you enter the trigger box at the end of the ramp.

## Why This Happens

- It appears that `OnRep_Controller` does not get called client-side for clients
  who are un-possessing a pawn. So, `Client #1` will not receive an
  `OnRep_Controller` callback for `Character A` when switching to control
  `Character B`. However, UE will eventually replicate `nullptr` to `Client #1`
  as the controller of `Character A` on the next tick, but without any
  RepNotify.
- ASC Ability Actor Info updates tend to happen in the same frame as the change
  in controllers, so the updated info will still record that the player
  controller for `Client #1` is the "owner" of Character A even though
  `Client #1` now possesses `Character B` instead. When it comes times to play
  the montage, the ASC thinks that the character is stll locally controlled and
  disregards the montage playback instruction from the server.
- The `Listen Server Client` is not subject to the bug because the controller of
  `Character A` is immediately set to `nullptr` during the controller change but
  before the ASC Ability Actor Info is refreshed in the same frame (there is no
  replication delay).
  
## The Workarounds

Looking back in commit history, this project demonstrates two approaches to work
around this issue:
1. **Blueprint-driven, Timer solution:** Whenever a player controller is
   switching from controlling `Pawn A` to `Pawn B`,
   [set a timer](https://github.com/OpenPF2/UE-279223-repro/commit/workaround-1)
   to refresh the ASC Ability Actor Info on `Pawn A` after a brief delay (0.1 to 0.5 secs);  
OR
2. **C++-driven, Timer Manager solution in `SetPawn`:** In 
   [the player controller](https://github.com/OpenPF2/UE-78453-repro/blob/workaround-2/Source/PossessionIssue/PossessionIssuePlayerController.cpp#L35),
   override the `SetPawn()` method so you can capture the old pawn before it
   gets replace with the new pawn. Then, use the `TimerManager` to schedule an
   ASC Ability Actor Info update on the next tick.
   
In both cases, you are ensuring that the controller info has replicated to the
client before the ASC has captured it.
