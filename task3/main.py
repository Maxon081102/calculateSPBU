from utils import RU, ENG, get_real_word

if __name__ == "__main__":
    while True:
        word = input()
        word_type_email = "@" in word
        if word_type_email:
            print(get_real_word(word, ENG, email=True))
        else:
            print(get_real_word(word, RU, email=False))
