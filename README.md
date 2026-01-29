# Intro Arcade (Week 2 update)

## Game rules
- Collect coins to score
- Avoid obstacles or lose a life
- Game ends when you lose 3 lives
- The more coins you collect, the more obstacles will appear
- Some obstacles may follow you!

## Run

```bash
python -m pip install pygame
python main.py
```

## Controls
- Arrow keys / WASD: move
- Enter: start / restart
- Esc: quit

## Save data
Writes `save.json` (high score only). Delete it to reset.

## This week
- New enemy type: seekers
- Lives system

## Design decisions
Seeker
- "Follow" player's movement, also much faster than bouncer
- Forces player to keep moving, makes staying still more dangerous
- Also creates focal points for attention, making bouncers more dangerous even for experienced players

Lives
- Extends gameplay by allowing mistakes before reset
- Smooths learning curve, as difficulty ramps, lives will be spent less frequently on early stages
- HUD element communicates system quickly and provides visual feedback on mistakes
