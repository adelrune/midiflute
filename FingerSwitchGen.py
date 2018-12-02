import sys

# Transforms a fingering file into the appropriate c code.

file_name = sys.argv[1]
current_note = -1
current_mapping = ""
mapping_symbols = [str(i) for i in range(10)]
mapping_symbols.extend(["+", "-", "("])

#expands the don't cares in the fingerings
def expand_d(fingering):
    if fingering.count("d") == 0:
        val = set()
        val.add(fingering)
        return val
    else:
        x_fingering = fingering.replace("d", "x", 1)
        o_fingering = fingering.replace("d", "o", 1)
        return set().union(expand_d(x_fingering)).union(expand_d(o_fingering))

with open(file_name) as f:
    finger_mappings = {"notes": {},"modifiers": {}}
    for line in f:
        #ignore comments
        if line[0] == "#":
            continue
        #if the line is not a valid finger representation, it should be a new note number.
        if current_note == -1 or line[0] in mapping_symbols:
            current_note = line.split(":")[0]
            current_note = current_note.strip("\n")
            #if the first char is + or -, we define fingerings for a modifier
            if line[0] in ["+", "-"]:
                current_mapping = "modifiers"
            else:
                current_mapping = "notes"
            #Initialize empty list that will contain all the fingerings for that note/modifier.
            finger_mappings[current_mapping][current_note] = set()
        #if the line is a valid fingering representation
        #d means don't care
        elif line[0] in ["d", "x", "o" ,"|"]:
            fingerings = expand_d(line)
            for finger in fingerings:
                #sum of the "bits" of the fingering in base 2
                fingering_num = 0
                sanitized_string = "".join(filter(lambda x: x in ["x", "o"], finger))
                for i in range(len(sanitized_string)):
                    fingering_num += 2**i if sanitized_string[i] == "x" else 0
                #removes this particular fingering in the other notes
                for note in finger_mappings[current_mapping]:
                    if fingering_num in finger_mappings[current_mapping][note]:
                        finger_mappings[current_mapping][note].remove(fingering_num)
                #adds the fingering in the note -> [fingerings] dictionary.
                finger_mappings[current_mapping][current_note].add(fingering_num)


with open("out.c", 'w+') as f:
    ind_levl = 0

    def write_line(string):
        f.write(" "*(ind_levl * 4) + string + '\r\n')

    write_line("int midiNoteFromFingering(uint32_t fingering, uint32_t modifiers) {")
    ind_levl += 1
    write_line("int note = 0;")
    write_line("switch(fingering) {")
    ind_levl +=1
    for note in sorted(finger_mappings["notes"]):
        for fingering in sorted(finger_mappings["notes"][note]):
            write_line("case({}):".format(fingering))
        ind_levl +=1
        write_line("note = {};".format(note))
        write_line("break;")
        ind_levl -=1
    write_line("default:")
    ind_levl +=1
    write_line("note = lastUnmodifiedNote;")
    ind_levl -=2
    write_line("}")
    write_line("lastUnmodifiedNote = note;")
    write_line("switch(modifiers) {")
    ind_levl +=1
    for modifier in sorted([finger_mappings["modifiers"]]):
        for fingering in sorted(finger_mappings["modifiers"][modifier]):
            write_line("case({}):".format(fingering))
        ind_levl +=1
        write_line("note {}= {};".format(modifier[0], modifier[1:]))
        write_line("break;")
        ind_levl -=1
    ind_levl -=1
    write_line("}")
    write_line("return note;")
    ind_levl -=1
    write_line("}")

