# writeup

一問あたり７秒の制限時間で、かつアクセスするたびに迷路は変わるのでプログラムで解く。

手法としては、深さ優先探索、幅優先探索などを用いて解くことができる。

## 解法例

```python
import socket

HOST = "203.0.113.9"
PORT = 5005

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    for _ in range(5):
        data = s.recv(4096).decode().split("\n")

        maze = data[4:-2]  # 迷路の範囲を取り出す
        height = len(maze)
        width = len(maze[0])
        seen = [[False] * width for _ in range(height)]
        stack = [(1, 1, "")]

        while True:
            h, w, way = stack.pop()

            if maze[h][w] == "G":
                s.sendall("{}\n".format(way).encode())
                break

            seen[h][w] = True

            for nh, nw, ns in [(h - 1, w, "U"), (h, w + 1, "R"), (h + 1, w, "D"), (h, w - 1, "L")]:
                if (maze[nh][nw] == ' ' or maze[nh][nw] == "G") and not seen[nh][nw]:
                    stack.append((nh, nw, "".join([way, ns])))

    print(s.recv(4096).decode().split("\n")[1])
```
