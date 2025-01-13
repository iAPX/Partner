import berserk
import chess, chess.polyglot
import os.path
import random
import time
import threading
import sqlite3
import subprocess
import sys


# Initialize Lichess client, token should be passed as parameter
session = berserk.TokenSession(sys.argv[1])
client = berserk.Client(session)

seconds_per_move = {}


def safe_make_move(client, game_id, coup, retries=5):
    for attempt in range(retries):
        try:
            print("Send move to Lichess...")
            client.bots.make_move(game_id, coup)
            return True  # Move was successful
        except Exception as e:
            print(f"Unexpected error: {e}")
        time.sleep(0.1)  # Wait before retrying
    print("Failed to make move after retries.")
    return False


def find_opening(moves):
    board = chess.Board()
    
    # Replay all moves to reach the current position
    for move in moves:
        board.push_uci(move)  # Assumes UCI format (e.g., 'e2e4')
    
    # Access the opening book if exists
    library_filename = "./lichess.bin"
    if os.path.isfile(library_filename):
        # 1-lichess.bin
        with chess.polyglot.open_reader(library_filename) as reader:
            # Fetch entries matching the current board position
            entries = list(reader.find_all(board))
            
            if entries:
                print("Lichess.bin used : " + str(entries))
                # Choose the best move based on weight
                entries = sorted(entries, key=lambda entry: entry.weight, reverse=True)

                # Up to 3 entries, 50%, 35% and 15%, to make games more fun!
                if len(entries) == 1 or random.random() < 0.5:
                    return entries[0].move.uci()
                elif len(entries) == 2 or random.random() < 0.7:
                    return entries[1].move.uci()
                else:
                    return entries[2].move.uci()

    # 2-ouvertures.db
    ouvertures_filename = "./ouvertures.db"
    if os.path.isfile(ouvertures_filename):
        conn = sqlite3.connect(ouvertures_filename, isolation_level=None)
        cursor = conn.cursor()

        query = """
            SELECT coup.coup, coup.score
            FROM position
            JOIN coup ON position.id = coup.position_id
            WHERE position.fen = ?
            ORDER BY coup.score DESC
        """
        coups = cursor.execute(query, (board.fen(),)).fetchall()
        conn.close()

        if coups:
            print("Ouvertures.db used : " + str(coups))
            # return random.choice(coups)[0]
            if len(coups) == 1 or random.random() < 0.5:
                return coups[0][0]
            elif len(coups) == 2 or random.random() < 0.7:
                return coups[1][0]
            else:
                return coups[2][0]

        return None


def find_first_move():
    board = chess.Board()

    # Access the opening book if exists
    library_filename = "lichess.bin"
    if os.path.isfile(library_filename) is False:
        print("Opening book not found : " + library_filename)
        return None
    
    with chess.polyglot.open_reader(library_filename) as reader:
        # Fetch entries matching the current board position
        entries = list(reader.find_all(board))
        print("entries: " + str(entries))
        return random.choice(entries).move.uci()


class Game(threading.Thread):
    def __init__(self, game_id, is_white, seconds, **kwargs):
            super().__init__(**kwargs)
            self.game_id = game_id
            self.is_white = is_white
            self.seconds = seconds
            self.stream = client.bots.stream_game_state(game_id)
            self.current_state = next(self.stream)
            self.in_opening = True if seconds > 0 else False     # No new opening book if old school 90s play!
            self._stop = False

            if (is_white):
                # client.bots.post_message(self.game_id, "Je joue les blancs! I play whites!")
                print("Premier coup? ")
                coup = find_first_move()
                print("Premier coup de Partner: " + coup + "\n")
                # coup = input().strip()
                try:
                    client.bots.make_move(game_id, coup)
                except Exception as e:
                    print("Premier coup Exception : " + str(e))
            else:
                pass
                # client.bots.post_message(game_id, "Je joue les noirs! I play blacks!")


    def run(self):
        # Salue les spectateurs!
        if self.seconds == 0:
            client.bots.post_message(self.game_id, "Hello this is a real Chess Bot from 1994!", True)
        else:
            client.bots.post_message(self.game_id, "Hello this is a Chess Bot from 1994, improved!", True)

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

        # Logs if game != started
        if event['status'] != 'started':
            print("Fin de partie?!? status = " + event['status'])
            try:
                side = "white" if self.is_white else "black"
                with open("games.log", "a") as file:
                    file.write(side + ":" + str(event) + "\n")
            except Exception as e:
                print("Exception pendant log fin de partie: " + str(e) + "\n")
            finally:
                return

        if (len(moves) % 2 == 1) == self.is_white:
            # Not my turn!
            print("Not my turn!")
            return

        print("Ton coup? ")

        if self.in_opening:
            # Opening libraries
            start_time = time.perf_counter_ns()
            coup = find_opening(moves)
            end_time = time.perf_counter_ns()
            if coup is not None:
                # print("Opening found: " + coup)
                print("Coup Libraries: " + coup + ", temps: " + str((end_time - start_time) / 1_000_000) + "ms\n")
                client.bots.make_move(game_id, coup)
                return
            # Avoid losing time in opening again
            print("Sortie des bibliothèques d'ouverture")
            self.in_opening = False

            # Et on enregistre les coups!
            with open("sortie-libraries.moves", 'a+') as f:
                f.write(' '.join(moves) + "\n")
        
        start_time = time.perf_counter_ns()
        output = subprocess.check_output(
            ["../portable/partner.lichess", "lichess", str(self.seconds), event['moves']],
            encoding="ascii",
            errors="ignore",
            text=True
            )
        end_time = time.perf_counter_ns()

        score = output.split("||||")[1].strip()
        try:    
            score_int = int(score)
        except:
            score_int = 0

        coup = output.split("////", 1)[-1].strip().lower()
        print("Coup de Partner: " + coup + ", temps: " + str((end_time - start_time) / 1_000_000) + "ms, score = " + score + "\n")
        if coup == "@0@0":
            # Pas de coups
            pass
        elif score_int <= -3000 and score_int > -10000:
            client.bots.post_message(self.game_id, "Bravo! Thanks for this game.")
            client.bots.post_message(self.game_id, "Merci d'avoir suivi cette partie! Thanks for following this game.", True)
            client.bots.resign_game(game_id)
        else:
            ## coup = input().strip()
            try:
                # client.bots.make_move(game_id, coup)
                safe_make_move(client, game_id, coup, retries=5)
            except Exception as e:
                print("Coup erreur : " + str(e))


    def handle_chat_line(self, event, game_id):
        print("Message:" + str(event))


games = {}

def main():
    """Main function to run the bot."""
    global seconds_per_move

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

            # no bots
            """
            if event['challenge']['challenger']['title'] == 'BOT':
                print("Decline challenge bot : declined!")
                try:
                    client.bots.decline_challenge(event['challenge']['id'])
                except Exception as e:
                    print("Decline challenger bot exception : " + str(e))
                continue
            """

            if len(games) >= 10:
                print("Decline challenge : declined!")
                try:
                    client.bots.decline_challenge(event['challenge']['id'])
                except Exception as e:
                    print("Decline challenge 10 games exception : " + str(e))
                continue

            # Only regular Chess
            variant_key = event['challenge']['variant']['key']
            if variant_key != 'standard':
                print(f"Decline challenge non-standard '{variant_key}' : declined!")
                try:
                    client.bots.decline_challenge(event['challenge']['id'])
                except Exception as e:
                    print("Decline challenge non-standard exception : " + str(e))
                continue

            # No correspondence games, nor classical games
            speed = event['challenge']['speed']
            if speed == 'correspondence':
                print(f"Decline challenge speed '{speed}' : declined!")
                try:
                    client.bots.decline_challenge(event['challenge']['id'])
                except Exception as e:
                    print("Decline challenge correspondence exception : " + str(e))
                continue

            # Maximum 1 hour (and extensions)
            if event['challenge']['rated'] and event['challenge']['timeControl']['limit'] >= 3600:
                print(f"Decline rated challenge timeControl '{event['challenge']['timeControl']['show']}' : declined!")
                try:
                    client.bots.decline_challenge(event['challenge']['id'])
                except Exception as e:
                    print("Decline challenge speed exception : " + str(e))
                continue

            print("Challenge accepted!")
            try:
                client.bots.accept_challenge(event['challenge']['id'])
            except Exception as e:
                print("Accept challenge exception : " + str(e))

            print("Speed text: " + event['challenge']['timeControl']['show'])
            seconds_per_move[event['challenge']['id']] = 0
            if 'limit' in event['challenge']['timeControl']:
                seconds_per_move[event['challenge']['id']] = event['challenge']['timeControl']['limit'] / 30
                print("timeControl Limit : " + str(event['challenge']['timeControl']['limit']))
                if 'increment' in event['challenge']['timeControl']:
                    seconds_per_move[event['challenge']['id']] += event['challenge']['timeControl']['increment']
                    print("timeControl Increment : " + str(event['challenge']['timeControl']['increment']))
            print("Seconds per move : " + str(seconds_per_move[event['challenge']['id']]))

        elif event['type'] == 'gameStart':
            print("Game start!")
            print(event)
            game_id = event['game']['id']

            is_rated = event['game']['rated']
            is_bot = event['game']['opponent']['username'].startswith('BOT ')
            if is_bot:
                client.bots.post_message(game_id, "Hello fellow bot, I am a Chess Bot from 1994!")
            else:
                if is_rated:
                    client.bots.post_message(game_id, "For the real 90's experience, you should play a casual game. Rated games are played at the new improved level. Welcome!")
                else:
                    client.bots.post_message(game_id, "Let's enjoy the real 90's experience with my original bad chess level!")
            
            seconds =0
            if is_bot or is_rated:
                if game_id in seconds_per_move:
                    seconds = seconds_per_move[game_id]
                else:
                    seconds = event['game']['secondsLeft'] / 30
            print("Secondes par coup : " + str(seconds))

            game = Game(game_id, event['game']['color'] == 'white', seconds)
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
