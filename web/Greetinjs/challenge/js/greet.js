const greetings = [
    "Hello",
    "Hey",
    "Heyyy",
    "Hi",
    "How are you doing?",
    "How do you do?",
    "How have you been?",
    "How's it going",
    "Nice to meet you",
    "Pleased to meet you",
    "Sup",
    "What's up?",
    "Yo!",
    //"ISCCTF{S0m3t1m35_y0u_c4n_ge7_50m3_h1nt5_fr0m_7h3_l04d3d_r3s0urc35}"
]

const el = document.getElementById("greeting");
el.textContent = `🧽<\t${greetings[~~(Math.random() * greetings.length)]}`;