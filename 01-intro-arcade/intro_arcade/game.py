from __future__ import annotations

import json
import pygame
import math
import random
from abc import ABC, abstractmethod
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Colors:
    bg: tuple[int, int, int] = (22, 24, 28)
    panel: tuple[int, int, int] = (34, 38, 46)
    text: tuple[int, int, int] = (236, 239, 244)

    player: tuple[int, int, int] = (136, 192, 208)  # blue
    bouncer: tuple[int, int, int] = (191, 97, 106)  # red
    seeker: tuple[int, int, int] = (180, 142, 173)  # purple
    coin: tuple[int, int, int] = (235, 203, 139)    # gold

COLORS = Colors()
LIFE_SIZE = 14
LIFE_SPACING = 6


class Enemy(ABC):
    def __init__(self, rect: pygame.Rect, vel: pygame.Vector2, color: tuple[int, int, int]) -> None:
        self.rect = rect
        self.vel = vel
        self.color = color

    @abstractmethod
    def update(self, dt: float, bounds: pygame.Rect, player: pygame.Rect) -> None:
        raise NotImplementedError

    def draw(self, screen: pygame.Surface) -> None:
        pygame.draw.rect(screen, self.color, self.rect, border_radius=8)

class Bouncer(Enemy):
    def update(self, dt: float, bounds: pygame.Rect, player: pygame.Rect) -> None:
        self.rect.x += int(self.vel.x * dt)
        self.rect.y += int(self.vel.y * dt)

        if self.rect.left < bounds.left:
            self.rect.left = bounds.left
            self.vel.x *= -1
        if self.rect.right > bounds.right:
            self.rect.right = bounds.right
            self.vel.x *= -1
        if self.rect.top < bounds.top:
            self.rect.top = bounds.top
            self.vel.y *= -1
        if self.rect.bottom > bounds.bottom:
            self.rect.bottom = bounds.bottom
            self.vel.y *= -1

class Seeker(Enemy):
    def update(self, dt: float, bounds: pygame.Rect, player: pygame.Rect) -> None:
        self.rect.x += int(self.vel.x * dt)
        self.rect.y += int(self.vel.y * dt)

        hit_wall = False

        if self.rect.left < bounds.left:
            self.rect.left = bounds.left
            hit_wall = True
        elif self.rect.right > bounds.right:
            self.rect.right = bounds.right
            hit_wall = True

        if self.rect.top < bounds.top:
            self.rect.top = bounds.top
            hit_wall = True
        elif self.rect.bottom > bounds.bottom:
            self.rect.bottom = bounds.bottom
            hit_wall = True

        if hit_wall:
            # change direction toward player
            speed = self.vel.length()
            dx = player.centerx - self.rect.centerx
            dy = player.centery - self.rect.centery
            dist = math.hypot(dx, dy)
            if dist != 0:
                self.vel.x = (dx / dist) * speed
                self.vel.y = (dy / dist) * speed



class Game:
    def __init__(self) -> None:
        self.fps = 60
        self.w = 960
        self.h = 540
        self.screen = pygame.display.set_mode((self.w, self.h))
        self.font = pygame.font.SysFont(None, 24)
        self.big_font = pygame.font.SysFont(None, 48)

        self.save_path = Path(__file__).resolve().parent.parent / "save.json"
        self.high_score = self._load_high_score()

        self.state: str = "title"  # title | playing | gameover
        self._reset_run()

    def _load_high_score(self) -> int:
        if not self.save_path.exists():
            return 0
        try:
            raw = json.loads(self.save_path.read_text(encoding="utf-8"))
            return int(raw.get("high_score", 0))
        except Exception:
            return 0

    def _save_high_score(self) -> None:
        self.save_path.write_text(
            json.dumps({"high_score": int(self.high_score)}, indent=2, sort_keys=True) + "\n",
            encoding="utf-8",
        )

    def _reset_run(self) -> None:
        self.player = pygame.Rect(self.w // 2 - 16, self.h // 2 - 16, 32, 32)
        self.player_v = pygame.Vector2(0, 0)

        self.score = 0
        self.alive_time = 0.0
        self.lives = 3

        self.enemies: list[Enemy] = []
        for _ in range(3):
            self._spawn_bouncer()

        self.coin = self._spawn_coin()

    def _respawn_player(self) -> None:
        self.player.center = (self.w // 2, 60 + (self.h - 60) // 2)
        self.player_v.update(0, 0)

    def _spawn_bouncer(self) -> None:
        r = pygame.Rect(random.randrange(40, self.w - 40), random.randrange(80, self.h - 40), 48, 48)
        v = pygame.Vector2(random.choice([-1, 1]) * 225, random.choice([-1, 1]) * 225)
        self.enemies.append(Bouncer(r, v, COLORS.bouncer))

    def _spawn_seeker(self) -> None:
        r = pygame.Rect(random.randrange(40, self.w - 40), random.randrange(80, self.h - 40), 32, 32)
        v = pygame.Vector2(random.choice([-1, 1]) * 360, random.choice([-1, 1]) * 360)
        self.enemies.append(Seeker(r, v, COLORS.seeker))

    def _spawn_coin(self) -> pygame.Rect:
        # Keep coin away from top HUD area.
        return pygame.Rect(random.randrange(20, self.w - 20), random.randrange(90, self.h - 20), 18, 18)

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.event.post(pygame.event.Event(pygame.QUIT))

            if event.key == pygame.K_RETURN:
                if self.state in ("title", "gameover"):
                    self._reset_run()
                    self.state = "playing"

    def update(self, dt: float) -> None:
        if self.state != "playing":
            return

        self.alive_time += dt

        # Input: map keys -> direction.
        keys = pygame.key.get_pressed()
        input_x = 0.0
        input_y = 0.0
        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            input_x -= 1.0
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            input_x += 1.0
        if keys[pygame.K_UP] or keys[pygame.K_w]:
            input_y -= 1.0
        if keys[pygame.K_DOWN] or keys[pygame.K_s]:
            input_y += 1.0

        # Movement: velocity integrates into position; dt makes it frame-rate independent.
        speed = 360.0
        self.player_v.x = input_x * speed
        self.player_v.y = input_y * speed

        self.player.x += int(self.player_v.x * dt)
        self.player.y += int(self.player_v.y * dt)
        self.player.clamp_ip(pygame.Rect(0, 60, self.w, self.h - 60))

        # Enemies: update all enemy objects.
        bounds = pygame.Rect(0, 60, self.w, self.h - 60)
        for e in self.enemies:
            e.update(dt, bounds, self.player)

        # Collision: player with coin.
        if self.player.colliderect(self.coin):
            self.score += 1
            self.coin = self._spawn_coin()
            if self.score % 10 == 0:
                self._spawn_seeker()
            elif self.score % 5 == 0:
                self._spawn_bouncer()

        # Collision: player with enemies -> lose a life
        if any(self.player.colliderect(e.rect) for e in self.enemies):
            self.lives -= 1

            if self.lives > 0:
                self._respawn_player()
                return
            else:
                self.state = "gameover"
                if self.score > self.high_score:
                    self.high_score = self.score
                    self._save_high_score()

    def draw(self) -> None:
        self.screen.fill(COLORS.bg)

        if self.state == "title":
            self._draw_title()
        elif self.state == "playing":
            self._draw_playing()
        else:
            self._draw_gameover()

    def _draw_hud(self) -> None:
        panel = pygame.Rect(12, 12, 520, 40)
        pygame.draw.rect(self.screen, COLORS.panel, panel, border_radius=10)

        # Lives (left)
        x = panel.x + 12
        y = panel.y + panel.height // 2 - LIFE_SIZE // 2
        for _ in range(self.lives):
            pygame.draw.rect(self.screen, COLORS.player, pygame.Rect(x, y, LIFE_SIZE, LIFE_SIZE), border_radius=4)
            x += LIFE_SIZE + LIFE_SPACING

        # Score (right)
        text = f"Score: {self.score}  High: {self.high_score}"
        surf = self.font.render(text, True, COLORS.text)

        right_pad = 12
        text_x = panel.right - right_pad - surf.get_width()
        text_y = panel.y + panel.height // 2 - surf.get_height() // 2
        self.screen.blit(surf, (text_x, text_y))

    def _draw_playing(self) -> None:
        self._draw_hud()

        pygame.draw.rect(self.screen, COLORS.coin, self.coin, border_radius=7)
        for e in self.enemies:
            e.draw(self.screen)
        pygame.draw.rect(self.screen, COLORS.player, self.player, border_radius=8)

    def _draw_title(self) -> None:
        title = self.big_font.render("Intro Arcade", True, COLORS.text)
        hint = self.font.render("Move with arrows/WASD.  Avoid red.  Collect gold.", True, COLORS.text)
        hint2 = self.font.render("Press Enter to start.  Esc to quit.", True, COLORS.text)

        self.screen.blit(title, (self.w / 2 - title.get_width() / 2, 190))
        self.screen.blit(hint, (self.w / 2 - hint.get_width() / 2, 250))
        self.screen.blit(hint2, (self.w / 2 - hint2.get_width() / 2, 280))

    def _draw_gameover(self) -> None:
        title = self.big_font.render("Game Over", True, COLORS.text)
        msg = self.font.render(f"Score: {self.score}   High: {self.high_score}", True, COLORS.text)
        hint = self.font.render("Press Enter to play again.  Esc to quit.", True, COLORS.text)

        self.screen.blit(title, (self.w / 2 - title.get_width() / 2, 190))
        self.screen.blit(msg, (self.w / 2 - msg.get_width() / 2, 250))
        self.screen.blit(hint, (self.w / 2 - hint.get_width() / 2, 280))
