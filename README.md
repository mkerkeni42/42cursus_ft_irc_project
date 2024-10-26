# ft_irc: Join the Conversation (And Try Not to Get Kicked)! 🚀

## Overview 📄

This project was a fantastic dive into **System and Network Administration** and my first big project in **C++**! 
Working with my teammates, Yani K. and Emin A., made it even better, and I’m genuinely proud of our work together. 🤝  

We built an **IRC (Internet Relay Chat) server** that lets users connect, chat, join channels, and experience essential IRC features—just like the real thing, but created from scratch. 💬✨

My main role? Handling with Yani K. all the **commands** that make IRC, well… IRC! Commands like `NICK`, `JOIN`, `KICK`, and more became my personal playground as I coded them to bring a real IRC experience to life. 
It was challenging but incredibly satisfying seeing users interact with the server using the features I implemented.

## Technologies Used 💻
<img src="https://cdn.worldvectorlogo.com/logos/c.svg" alt="C++" width="40" height="40"/>

## How to Use ⚙️
To try out our IRC server, follow these steps:

1. **Compile** the project using `make`.

2. **Run the server** with your chosen port and password:
```
./ircserv <port> <password>
```

3. **Connect with an IRC Client** (we used `irssi` for reference):
```
irssi -c localhost -p <port> -w <password>
```

4. **Start chatting!**

## Challenges I Faced 💪
- **Implementing Core Commands**: Taking on commands like KICK, INVITE, TOPIC, and MODE was a complex process. Each command has its own nuances, especially with permissions and operator roles.
  Writing each from scratch while ensuring they work smoothly was a rewarding challenge.

- **Testing and Refinement**: Testing each command to ensure it mimicked real IRC behavior was meticulous. 🕵️‍♂️ This involved handling edge cases, like invalid inputs and command restrictions, to make sure everything functioned reliably for users.

- **Collaborating on I/O Operations**: While Emin handled the non-blocking I/O aspects, ensuring that commands integrated smoothly with his implementation required precise coordination.

## What I Learned 🧠
- **Command Structure and Parsing**: Diving deep into IRC commands taught me about parsing and structuring commands in real-time, a skill I know will come in handy in future projects.

- **C++ Networking Skills**: From sockets to managing permissions, I gained a solid foundation in C++ networking, which I’m excited to build on.

- **Team Collaboration**: Working alongside Yani and Emin was a highlight! Each of us tackled different project components, bringing them together to create a cohesive server. 🤗
It was a great lesson in dividing and conquering complex tasks as a team. 💪

In the end, ft_irc was an unforgettable project. The combination of implementing core IRC commands, mastering network programming in C++, and collaborating with my team made this project both challenging and incredibly rewarding. 🌟
