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
    while len(teams) > 1:
        next_teams = []
        half = len(teams) // 2
        for i in range(0, half):
            winner = play_match(state, teams[i].id, teams[half + i].id).value
            next_teams.append(state.team(winner))
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
    cmd = ['timeout', 'main.exe', '20']
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
        test = generate_input(start_N, inp_min, inp_max)
        py_out = run_test(test).replace('\r\n', '\n') + '\n'
        cpp_out = run_exe(test).replace('\r\n', '\n') + '\n'
        if py_out != cpp_out:
            return test
    return run_iterativly_until_error(
        start_N + 1,
        max_N,
        inp_min,
        inp_max,
    )

bad_test = run_iterativly_until_error(10, 100, 1, 5)
if bad_test:
    run_single(bad_test)
