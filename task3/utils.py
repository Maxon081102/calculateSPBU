RU = set("".join([chr(i) for i in range(ord("а"), ord("я") + 1)]))
ENG = set("".join([chr(i) for i in range(ord("a"), ord("z") + 1)]))

RU_SIZE = len(RU)
ENG_SIZE = len(ENG)

assert RU_SIZE == 32 and ENG_SIZE == 26


def diff(a, b):
    return ord(a) - ord(b)


def find_shift_email(email) -> int:
    COM = "com"

    def get_local_diff(word, number):
        return (diff(word[number], COM[number]) + ENG_SIZE) % ENG_SIZE

    prepared_email = email.lower().split(".")
    last_word = prepared_email[-1]
    first_diff = get_local_diff(last_word, 0)
    second_diff = get_local_diff(last_word, 1)
    third_diff = get_local_diff(last_word, 2)
    if first_diff == second_diff and first_diff == third_diff:
        return first_diff
    return diff(last_word[0], "o")


def find_shift_address(address) -> int:
    prepared_address = address.lower().strip("0123456789.- ")
    return diff(prepared_address.split()[-1][-1], "в")


def get_real_letter(letter, shift, LANGUAGE):
    letter_lower = letter.lower()
    if letter_lower not in LANGUAGE:
        return letter

    SIZE = len(LANGUAGE)
    real_letter = chr(
        (ord(letter_lower) - ord(min(LANGUAGE)) - shift + SIZE) % SIZE
        + ord(min(LANGUAGE))
    )
    if letter == letter_lower:
        return real_letter
    return real_letter.upper()


def get_real_word(word, LANGUAGE, email=True):
    shift = find_shift_email(word) if email else find_shift_address(word)
    return "".join([get_real_letter(letter, shift, LANGUAGE) for letter in word])


def get_shifted_language(word, LANGUAGE, email=True):
    shift = find_shift_email(word) if email else find_shift_address(word)
    return "".join(
        [
            get_real_letter(chr(letter), -shift, LANGUAGE)
            for letter in range(ord(min(LANGUAGE)), ord(max(LANGUAGE)) + 1)
        ]
    )
