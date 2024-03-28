from dataclasses import dataclass
from typing import Callable, List
from random import randint, choice



@dataclass
class Command:
    arity: int
    function: Callable

@dataclass
class Player:
    strength: int

    def show(self): return str(self.strength)

@dataclass
class Team:
    id: int
    wins: int
    players: List[Player]

    def strength(self):
        if not self.players:
            return 0
        median_player = sorted(self.players, key=lambda player: player.strength)[len(self.players) // 2]
        return median_player.strength * len(self.players)

    def rank(self):
        # Edge cases
        if len(self.players) == 0:
            return 0
        return self.strength() + self.wins

    def show(self): return f"Team {self.id} w={self.wins} [{','.join(p.show() for p in self.players)}]"

@dataclass
class State:
    teams: List[Team]

    def team_exists(self, team_id: int):
        return any(team.id == team_id for team in self.teams)

    def team(self, team_id: int):
        return next(team for team in self.teams if team.id == team_id)

    def show(self): return 'State:\n\t' + '\n\t'.join(t.show() for t in self.teams)

@dataclass
class Output:
    status: str
    value: int

SUCCESS = 'SUCCESS'
INVALID_INPUT = 'INVALID_INPUT'
FAILURE = 'FAILURE'


def add_team(state: State, team_id: int):
    if team_id <= 0:
        return INVALID_INPUT
    if state.team_exists(team_id):
        return FAILURE
    state.teams.append(Team(team_id, 0, []))
    return SUCCESS

def remove_team(state: State, team_id: int):
    if team_id <= 0:
        return INVALID_INPUT
    if not state.team_exists(team_id):
        return FAILURE
    state.teams = [team for team in state.teams if team.id != team_id]
    return SUCCESS

def add_player(state: State, team_id: int, player_strength: int):
    if player_strength <= 0 or team_id <= 0:
        return INVALID_INPUT
    if not state.team_exists(team_id):
        return FAILURE
    team = state.team(team_id)
    team.players.append(Player(player_strength))
    return SUCCESS

def remove_newest_player(state: State, team_id: int):
    if team_id <= 0: return INVALID_INPUT
    if not state.team_exists(team_id): return FAILURE
    if len(state.team(team_id).players) == 0: return FAILURE

    team = state.team(team_id)
    team.players.pop()
    return SUCCESS

def play_match(state: State, team_id1: int, team_id2: int):
    if team_id1 <= 0 or team_id2 <= 0 or team_id1 == team_id2:
        return Output(INVALID_INPUT, 0)
    if not state.team_exists(team_id1) or not state.team_exists(team_id2):
        return Output(FAILURE, 0)
    if state.team(team_id1).players == [] or state.team(team_id2).players == []:
        return Output(FAILURE, 0)

    team1 = state.team(team_id1)
    team2 = state.team(team_id2)
    winner = team1.id
    if team1.strength() > team2.strength():
        team1.wins += 1
    elif team2.strength() > team1.strength():
        team2.wins += 1
        winner = team2.id
    elif team1.id < team2.id:
        team1.wins += 1
    else:
        team2.wins += 1
        winner = team2.id
    return Output(SUCCESS, winner)

def num_wins_for_team(state: State, team_id: int):
    if team_id <= 0: return Output(INVALID_INPUT, 0)
    if not state.team_exists(team_id): return Output(FAILURE, 0)
    team = state.team(team_id)
    return Output(SUCCESS, team.wins)

# This should actually return the highest rank, not the team itself
def get_highest_ranked_team(state: State):
    if not state.teams: return Output(SUCCESS, -1)
    r = max(state.teams, key=lambda team: team.rank()).rank()
    return Output(SUCCESS, r)

def unite_teams(state: State, team_id1: int, team_id2: int):
    if team_id1 <= 0 or team_id2 <= 0 or team_id1 == team_id2:
        return INVALID_INPUT
    if not state.team_exists(team_id1) or not state.team_exists(team_id2):
        return FAILURE
    team1 = state.team(team_id1)
    team2 = state.team(team_id2)
    team1.players.extend(team2.players)
    remove_team(state, team_id2)
    return SUCCESS

def play_tournament(state: State, low_power: int, high_power: int):
    if low_power <= 0 or high_power <= 0 or low_power > high_power:
        return INVALID_INPUT
    teams = [team for team in state.teams if low_power <= team.strength() <= high_power]
    if len(teams).bit_count() != 1:
        return FAILURE
    teams.sort(key=lambda team: (team.strength(), -team.id))
    while len(teams) > 1:
        next_teams = []
        half = len(teams) // 2
        for i in range(0, half):
            winner = play_match(state, teams[i].id, teams[half + i].id).value
            next_teams.append(state.team(winner))
        teams = next_teams
    return Output(SUCCESS, teams[0].id)

commands = {
    'add_team': Command(1, add_team),
    'remove_team': Command(1, remove_team),
    'add_player': Command(2, add_player),
    'remove_newest_player': Command(1, remove_newest_player),
    'play_match': Command(2, play_match),
    'num_wins_for_team': Command(1, num_wins_for_team),
    'get_highest_ranked_team': Command(0, get_highest_ranked_team),
    'unite_teams': Command(2, unite_teams),
    'play_tournament': Command(2, play_tournament),
}


def run_exe(stdin: str) -> str:
    import subprocess
    cmd = ['main.exe']
    p = subprocess.run(cmd, capture_output=True, input=stdin.encode())
    return p.stdout.decode().strip()


def generate_command(inp_min=-1, inp_max=2):
    command_keys = list(commands.keys())
    command_keys += 3 * ['add_team', 'add_player']
    command_key = choice(command_keys)
    command = commands[command_key]
    args = [ randint(inp_min, inp_max) for _ in range(command.arity) ]
    return f'{command_key} {" ".join(map(str, args))}'

def generate_input(length=20, inp_min=-1, inp_max=2):
    N = length
    return '\n'.join([generate_command(inp_min=inp_min, inp_max=inp_max) for _ in range(N)])

def run_command(state: State, line: str) -> str:
    command_name = line.split()[0]
    args = list(map(int, line.split()[1:]))
    command = commands[command_name]
    out = command.function(state, *args)
    if isinstance(out, Output):
        if out.status == SUCCESS:
            return f'{command_name}: {out.status}, {out.value}'
        else:
            return f'{command_name}: {out.status}'
    else:
        return f'{command_name}: {out}'

def run_test(inp: str, trace=False) -> str:
    state = State([])
    output = []
    for line in inp.splitlines():
        output.append(run_command(state, line))
        if trace:
            print(line)
            print(state.show())
    return '\n'.join(output)


def run_single(inp = generate_input()):
    LAST_INPUT = 'last_input.txt'
    with open(LAST_INPUT, 'w') as f:
        f.write(inp)

    py_out = run_test(inp).replace('\r\n', '\n') + '\n'
    cpp_out = run_exe(inp).replace('\r\n', '\n') + '\n'

    import difflib

    d = difflib.ndiff(
        py_out.splitlines(keepends=True),
        cpp_out.splitlines(keepends=True),
    )
    print('Showing diff between outputs for a random test!')
    print(f'Original test input can be seen in {LAST_INPUT}')
    print('Python output is the original (-) and cpp implmentation is the changes (+)!')
    print(''.join(d))

    fail = py_out != cpp_out

    if fail:
        print()
        print('Uh-oh!')
        print('You failed this test. Run again with state trace? (y/n)', end=' ')
        c = None
        while c not in {'y', 'n'}:
            c = input()

        if c == 'y':
            run_test(inp, True)


def run_iterativly_until_error(start_N = 1, max_N = 100, inp_min=-1, inp_max=2):
    if start_N > max_N: return None

    print(f'Testing with N = {start_N}')
    
    Q = (inp_max - inp_min + 1) * start_N * commands.keys().__len__()
    for _ in range(0, Q):
        print('.', end='', flush=True)
        test = generate_input(start_N, inp_min, inp_max)
        py_out = run_test(test).replace('\r\n', '\n') + '\n'
        cpp_out = run_exe(test).replace('\r\n', '\n') + '\n'
        if py_out != cpp_out:
            print('!')
            return test
    print('!')
    return run_iterativly_until_error(
        start_N + 1,
        max_N,
        inp_min,
        inp_max,
    )

inp = '''
add_team 39
add_player 39 4715
add_player 39 644
add_team 194
add_team 69
add_player 39 2671
add_player 69 7945
add_player 39 7093
add_team 199
add_player 69 3364
add_player 69 9662
add_team 46
add_team 104
add_player 194 9916
add_player 46 8157
add_player 194 7613
add_team 153
add_player 194 1469
add_player 194 506
add_team 67
add_player 153 221
add_player 104 9767
add_player 104 3153
add_player 39 4817
add_team 74
add_team 15
get_highest_ranked_team
play_match 153 194
unite_teams 193 199
play_match 104 39
play_match 69 153
unite_teams 46 74
add_player 39 1868
num_wins_for_team 39
remove_newest_player 104
add_team 24
add_player 104 1367
num_wins_for_team 127
num_wins_for_team 24
remove_team 61
unite_teams 67 39
play_tournament 20856 28858
unite_teams 113 24
unite_teams 67 69
play_tournament 49 69
remove_newest_player 101
remove_newest_player 194
add_player 93 201
get_highest_ranked_team
remove_team 153
num_wins_for_team 107
add_player 199 7608
remove_team 93
remove_team 46
num_wins_for_team 174
play_tournament 8054 25116
play_tournament 8881 41283
add_player 44 9567
get_highest_ranked_team
get_highest_ranked_team
get_highest_ranked_team
play_tournament 117 252
unite_teams 104 24
get_highest_ranked_team
add_team 169
num_wins_for_team 194
remove_newest_player 169
play_match 5 12
get_highest_ranked_team
remove_team -18
add_player -7 7146
play_tournament 191 207
get_highest_ranked_team
add_team 183
unite_teams 152 165
add_player 104 9480
remove_team 169
remove_team 104
get_highest_ranked_team
add_team 187
play_match 194 67
unite_teams 183 187
remove_team 194
add_player 67 1431
add_player 6 1507
add_player 184 4791
unite_teams 183 81
unite_teams 183 67
remove_team 15
play_tournament 397 534175462
num_wins_for_team 199
get_highest_ranked_team
get_highest_ranked_team
play_tournament 1514 339495265
remove_newest_player 183
get_highest_ranked_team
add_player 199 3488
remove_newest_player 199
add_player 183 5278
play_tournament 5987 151233040
play_match 183 199
add_player 183 1867
add_player 104 5987
num_wins_for_team 199
remove_team 199
remove_team 21
remove_newest_player 183
remove_team 183
play_tournament 172 342
play_tournament 197 313
remove_newest_player -6
add_team 84
add_team 37
num_wins_for_team 199
remove_newest_player -20
get_highest_ranked_team
remove_team 84
get_highest_ranked_team
remove_team 37
add_player -15 -18
play_match 157 -4
add_team 200
add_player 66 9322
unite_teams 181 200
num_wins_for_team 200
play_tournament 36 200
add_player 200 1177
num_wins_for_team 174
add_player 200 4379
add_player 20 4530
add_team 115
unite_teams 102 115
play_tournament 138 233
add_player -20 -9
add_player 24 8526
get_highest_ranked_team
add_team 200
play_tournament 158 293
remove_newest_player 200
unite_teams 87 115
play_match -5 37
remove_newest_player 200
get_highest_ranked_team
num_wins_for_team 30
remove_team 200
unite_teams 198 146
num_wins_for_team 88
unite_teams 58 115
num_wins_for_team 48
remove_team 115
num_wins_for_team 54
remove_team 21
unite_teams 116 182
remove_team 82
play_match 1 -4
add_team 80
get_highest_ranked_team
add_player 80 5907
unite_teams 160 80
play_match -11 1
add_player 80 8074
unite_teams 80 119
remove_team 80
get_highest_ranked_team
add_team 92
play_tournament 184 222
get_highest_ranked_team
unite_teams 53 53
add_team 92
add_team 15
remove_team 48
remove_newest_player 15
play_match 83 15
play_match 92 15
add_player 15 9440
play_tournament 95 234
play_match -2 -2
play_match 28 -12
remove_newest_player 15
play_match 110 110
play_match 51 51
get_highest_ranked_team
get_highest_ranked_team
num_wins_for_team 59
play_match 15 92
remove_newest_player -6
remove_newest_player -18
remove_newest_player 63
unite_teams 30 157
add_player -10 -8
play_match -11 174
play_tournament -11 -17
play_tournament 8601 -15
add_team 51
play_tournament 7908 1745
add_team -1
remove_team 92
play_tournament 80 85
num_wins_for_team -20
play_tournament 3116 -4
'''.strip()

run_single(inp)
print('Test passed!')
exit()

bad_test = run_iterativly_until_error(10000, 20000, 1, 5)
if bad_test:
    run_single(bad_test)
