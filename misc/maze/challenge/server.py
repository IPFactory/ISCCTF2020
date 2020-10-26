import socket
import socketserver
from maze import Maze


HOST, PORT = "", 16800
FLAG = "ISCCTF{y0u_r4n_thr0ugh_the_M4ZE!}"

class ChallengeHandler(socketserver.BaseRequestHandler):
    def handle(self):
        client = self.request
        client.settimeout(7)
        maze_size = [5, 9, 17, 25, 33]

        for i in range(len(maze_size)):
            m = Maze(maze_size[i]*2+1, maze_size[i])
            m.set_start(1, 1, "S")
            m.set_goal(-2, -2, "G")
            m.solve()

            client.send("\nChallenge #{}\n\n{}\n{}\n\nanswer:".format(i+1, maze_size[i], m.to_str()).encode())
            print(m.answer)

            try:
                recv_ans = client.recv(4096).decode().strip()
                if m.answer != recv_ans:
                    client.send("Wrong answer, bye.\n".encode())
                    return
            except socket.timeout:
                client.send("\nTimeout, bye.\n".encode())
                return

        client.send("Congratulations!\n{}\n".format(FLAG).encode())
        return


if __name__ == "__main__":
    server = socketserver.ThreadingTCPServer((HOST, PORT), ChallengeHandler)
    server.serve_forever()

