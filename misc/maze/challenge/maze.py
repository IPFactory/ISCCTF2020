#!/usr/bin/env python3

from random import randint, sample, choice
from collections import defaultdict
from math import ceil

class Maze:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.maze = []
        self.wall = "#"
        self.passage = " "
        self.num_roads = defaultdict()
        self.create_maze()

    def create_maze(self):
        if self.width < 5 or self.height < 5:
            print("5以上の数値を入力してください")
            return
        if not self.width & 1:
            self.width += 1
        if not self.height & 1:
            self.height += 1

        self.maze = [[self.wall]*(self.width) for _ in range(self.height)]
        y = randint(0, self.height // 2 - 1) * 2 + 1
        x = randint(0, self.width // 2 - 1) * 2 + 1

        self.num_roads[(y,x)] = 0

        while self.num_roads:
            y, x = sample(self.num_roads.keys(), 1)[0]
            self.maze[y][x] = self.passage

            next_path = self._get_next_path(y,x)

            while next_path:
                self.num_roads[(y,x)] = len(next_path)
                        
                next_y, next_x = choice(next_path)
                self.maze[next_y][next_x] = self.maze[ceil((y + next_y) / 2)][ceil((x + next_x) / 2)] = self.passage

                x = next_x
                y = next_y
                
                self._num_roads_decrement(next_y, next_x)
                next_path = self._get_next_path(y,x)

    def _get_next_path(self, y: int, x: int):
        res = []
        for ny, nx in [(y - 2, x), (y + 2, x), (y, x - 2), (y, x + 2)]:
            if 0 < ny < self.height and 0 < nx < self.width and self.maze[ny][nx] == self.wall:
                res.append((ny, nx))
        return res
    
    def _num_roads_decrement(self, y: int, x: int):
        for ny, nx in [(y - 2, x), (y + 2, x), (y, x - 2), (y, x + 2)]:
            if 0 < ny < self.height and 0 < nx < self.width and self.maze[ny][nx] == self.passage:
                if (ny, nx) in self.num_roads:
                    self.num_roads[(ny, nx)] -= 1
                    if self.num_roads[(ny, nx)] < 1:
                        self.num_roads.pop((ny, nx))

    def to_str(self):
        return "\n".join(["".join(l) for l in self.maze])
    
    def set_start(self, h, w, s):
        if self.maze[h][w] == self.passage:
            self.maze[h][w] = s
            self.start_str = s
            self.start = (h, w)
        else:
            print("道を選択してください")
            return
    
    def set_goal(self, h, w, s):
        if self.maze[h][w] == self.passage:
            self.maze[h][w] = s
            self.goal_str = s
            self.goal = (h, w)
        else:
            print("道を選択してください")
            return
    
    def solve(self):
        if "start" not in dir(self) or "goal" not in dir(self):
            print("スタートとゴールを設定してください")
            return

        seen = [[False]*self.width for _ in range(self.height)]
        stack = [(self.start[0], self.start[1], '')]

        while stack:
            h, w, s = stack.pop()

            if self.maze[h][w] == self.goal_str:
                self.answer = s
                return
            
            seen[h][w] = True

            for nh, nw, ns in [(h - 1, w, "U"), (h, w + 1, "R"), (h + 1, w, "D"), (h, w - 1, "L")]:
                if 0 < nh < self.height and 0 < nw < self.width and (self.maze[nh][nw] == ' ' or self.maze[nh][nw] == self.goal_str) and not seen[nh][nw]:
                    stack.append((nh, nw, "".join([s, ns])))
