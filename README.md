# IRC

The project `IRC` involves creating an Internet Relay Chat client in C, allowing users to connect to an IRC server, join channels, and send/receive messages in real-time. It's **the third group project** of the common core, my mates were **mafissie** and **slepetit**.

## Features

- Connect to an IRC server
- User registration and identification
- Join and leave channels
- Send and receive private and group messages
- User management (list of online users)
- Support for basic IRC commands (KICK, INVITE, TOPIC, MODE -itkol, JOIN, PRIVMSG)


## Installation

To clone this repository and run the projects locally, use the following command:

   ```bash
   git clone https://github.com/username/irc-42.git
   cd irc-42
   ```

## Usage

  ```bash
   ./ircserv <port> <password>
  ```
- `<port>`: The number of the port on which your server will accept incoming connections.
- `<password>`: The password to log in to your IRC server, which must be provided by any IRC client that wants to connect.
