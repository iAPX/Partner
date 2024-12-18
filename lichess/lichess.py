import berserk
import time
import threading
import subprocess
import sys


# Initialize Lichess client, token should be passed as parameter
session = berserk.TokenSession(sys.argv[1])
client = berserk.Client(session)

def handle_game(game_id):
    """Handles a single game."""
    game_stream = client.bots.stream_game_state(game_id)
    client.bots.post_message(game_id, "Hello this is a test Chess Bot!")
    # game_stream = client.board.stream_game_state(game_id)
    for event in game_stream:
        print(f"Game Stream Event type: {event['type']}")
        if event['type'] == 'gameFull' or event['type'] == 'gameState':
            moves = event['moves'].split()
            if len(moves) > 10:
                client.bots.post_message(game_id, "10 coups atteints, je perd!")
                client.bots.resign_game(game_id)
                return
            if len(moves) % 2 == 0:  # It's the bot's turn
                # Generate a move (for now, just play the first legal move)
                board_state = client.board.get_legal_moves(game_id)
                if board_state:
                    move = board_state[0]  # Pick the first move
                    client.bots.make_move(game_id, move)


class Game(threading.Thread):
    def __init__(self, client, game_id, is_white, **kwargs):
            super().__init__(**kwargs)
            self.game_id = game_id
            self.client = client
            self.is_white = is_white
            self.stream = client.bots.stream_game_state(game_id)
            self.current_state = next(self.stream)
            self._stop = False

            if (is_white):
                client.bots.post_message(game_id, "Je joue les blancs!")
                print("Premier coup? ")
                output = subprocess.check_output(
                    ["../portable/partner", "lichess", ""],
                    encoding="ascii",
                    errors="ignore",
                    text=True
                    )
                coup = output.split("////", 1)[-1].strip().lower()
                print("Premier coup de Partner: " + coup + "\n")
                # coup = input().strip()
                client.bots.make_move(game_id, coup)

    def run(self):
        client.bots.post_message(self.game_id, "Hello this is a test Chess Bot!")
        for event in self.stream:
            print("Game Event type: " + event['type'])
            if event['type'] == 'gameState':
                self.handle_state_change(event, self.game_id)
            elif event['type'] == 'chatLine':
                self.handle_chat_line(event, self.game_id)

            # Finish
            if self._stop:
                print("Sortie du jeu!")
                break

    def handle_state_change(self, event, game_id):
        print(event)
        moves = event['moves'].split()
        print(moves)

        if (len(moves) % 2 == 1) == self.is_white:
            # Not my turn!
            print("Not my turn!")
            return
        
        print("Ton coup? ")
        output = subprocess.check_output(
            ["../portable/partner", "lichess", event['moves']],
            encoding="ascii",
            errors="ignore",
            text=True
            )
        coup = output.split("////", 1)[-1].strip().lower()
        print("Coup de Partner: " + coup + "\n")
        if coup == "@0@0":
            pass
            # client.bots.resign_game(game_id)
        else:
            ## coup = input().strip()
            client.bots.make_move(game_id, coup)


    def handle_chat_line(self, event, game_id):
        pass


games = {}

def main():
    """Main function to run the bot."""
    print("Bot is running...")
    # Stream incoming events
    start_time = time.time()
    for event in client.bots.stream_incoming_events():
        # @TODO for the firsts 3 seconds, deny challenges and resign on every gameStart!
        print(f"Event type: {event['type']}")

        if time.time() - start_time <= 5:
            if event['type'] == 'challenge':
                print(f"5s / Challenge {event['challenge']['id']} declined!")
                client.bots.decline_challenge(event['challenge']['id'])
            if event['type'] == 'gameStart':
                game_id = event['game']['id']
                print(f"5s / Resign game {game_id}")
                client.bots.resign_game(game_id)
            else:
                print(f"5s / Unknown Event type: {event['type']}")
            continue

        if event['type'] == 'challenge':
            print(f"Challenge {event['challenge']['id']}")
            print(event)

            if len(games) >= 10:
                print("Decline challenge : declined!")
                client.bots.decline_challenge(event['challenge']['id'])
                continue

            variant_key = event['challenge']['variant']['key']
            if variant_key != 'standard':
                print(f"Decline challenge non-standard '{variant_key}' : declined!")
                client.bots.decline_challenge(event['challenge']['id'])
                continue

            print("Challenge accepted!")
            client.bots.accept_challenge(event['challenge']['id'])

        elif event['type'] == 'gameStart':
            print("Game start!")
            print(event)
            game_id = event['game']['id']

            game = Game(client, game_id, event['game']['color'] == 'white')
            games[game_id] = game
            game.start()

        elif event['type'] == 'gameFinish':
            game_id = event['game']['id']
            print("gameFinish : " + game_id)
            print(event)
            if game_id not in games:
                print("gameFinish : " + game_id + " not in games")
                continue
            print(event)
            games.pop(game_id)

        else:
            print("unknown stream event : " + event['type'])
            print(event)
            pass

if __name__ == "__main__":
    main()
