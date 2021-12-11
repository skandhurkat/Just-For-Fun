import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--required_letter", required=True)
    parser.add_argument("--other_letters", nargs="+", required=True)
    parser.add_argument("--word_list", required=True)
    parser.add_argument("--min_size", default=1, type=int)

    args = parser.parse_args()

    assert len(args.required_letter) == 1
    required_letter = set(args.required_letter[0])
    allowed_letters = set([l.lower() for l in args.other_letters]) | required_letter

    words_found = 0

    with open(args.word_list) as word_list_file:
        for word in word_list_file:
            word = word.rstrip()
            if len(word) < args.min_size:
                continue
            letter_set = set(word.lower())
            if not required_letter.issubset(letter_set):
                continue
            if letter_set.issubset(allowed_letters):
                words_found += 1
                print(word)


    print(f"Found {words_found} words")
