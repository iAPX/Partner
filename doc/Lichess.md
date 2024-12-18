# Lichess Chess Bot integration

That was my first goal : integrating this old and ugly chess engine from '94 into Lichess.<br/>
Lichess is French (cocorico!), free and community oriented and moreover open-source: everything is aligned.<br/>


## Requirements
You should have compiled partner with gcc for your platform. [gcc compilation doc](./GCC-compilation.md)

You need Python 3.8+ and do a `pip install berserk`.<br/>
I recommend using a virtual env and updating url3lib and requests.


## Lichess Chess Bot account
[Some help here](https://lichess.org/forum/general-chess-discussion/how-to-make-a-chess-bot?page=2).

Create a new Chess account on Lichess. And another for yourself.<br/>
Don't play *ANY* game with this account, block challenges (NEVER).<br/>
Befriend your personal account from your chessbot account, and reciprocally.<br/>
Block every following.

Create an API token for your Chess Bot Account.<br/>
You need `bot: api`, `challenge:read`, `challenge:write` permissions

Now, update your Bot Chess account to "bot" type :<br/>
`curl -d '' lichess.org/api/bot/account/upgrade -H "Authorization: Bearer {YourToken}"`


## Running as Lichess Chess Bot
As simple as that on the [lichess directory](../lichess/)<br/>
`python3 lichess.py {YourToken}`


## Logs
Logs are recorded after each game on [lichess/games.log](../lichess/games.log)<br/>
It's the side of Partner (either "white" or "black") followed by a colon ":" and the event as a string. With a NewLine \n<br/>

Example:
```
black:{'type': 'gameState', 'moves': 'd2d4 g8f6 f2f4 d7d6 e2e4 f6e4', 'wtime': datetime.datetime(1970, 1, 25, 20, 31, 23, 647000, tzinfo=datetime.timezone.utc), 'btime': datetime.datetime(1970, 1, 25, 20, 31, 23, 647000, tzinfo=datetime.timezone.utc), 'winc': datetime.datetime(1970, 1, 1, 0, 0, tzinfo=datetime.timezone.utc), 'binc': datetime.datetime(1970, 1, 1, 0, 0, tzinfo=datetime.timezone.utc), 'status': 'resign', 'winner': 'black'}
```
