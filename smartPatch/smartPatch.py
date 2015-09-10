import difflib

def readString(filename):
  f = open(filename, "r")
  ret = f.read()
  f.close()
  return ret

def writeString(filename, fileStr):
  f = open(filename, "w")
  f.write(fileStr)
  f.close()

def assertPrint(lines, i, input_str, in_line, tmp_str, tmp_line, output_str):
    print "invalid %d:"%i, lines[i], lines[i + 1]
    print "input %d:"%in_line, input_str[in_line]
    print "tmp %d:"%tmp_line, tmp_str[tmp_line]
    print output_str
    assert(False)

def smartPatch(filename):
  INPUT_DIR = "./input/"
  TEMPLATE_DIR = "./template/"
  OUTPUT_DIR = "./output/"
  input_str = readString(INPUT_DIR + filename).splitlines()
  tmp_str = readString(TEMPLATE_DIR + filename).splitlines()

  d = difflib.Differ()
  diff = d.compare(input_str, tmp_str)
  
  output_str = []
  lines = list(diff)
  print "--patch string --"
  print "\n".join(lines)
  print "----"
  in_line = 0
  tmp_line = 0
  startline = None

  m = difflib.SequenceMatcher()

  for i in range(len(lines)):
    command = lines[i][0]
    line = lines[i][2:]
    if(startline != None):
      if(command == " " and line == input_str[startline + 1]):
        for j in range(in_line, startline + 1):
          output_str.append(input_str[j])
        in_line = startline + 1
        while True:
          tmp_line += 1
          if(tmp_str[tmp_line][:2] == "+_"):
            output_str.append(tmp_str[tmp_line][2:])
          else:
            break
        startline = None

      else:
        print "ignore patch", lines[i]
        continue
    if(command == "-"):
      output_str.append(line)
      in_line += 1
    elif(command == "+"):
      if(line[:2] == "+_"):
        output_str.append(line[2:])
        tmp_line += 1
      elif(line[:2] == "*_"):
        if(i + 1 < len(lines)):
          if(lines[i + 1][0] == "?"):
            print "sentinel found"
          else:
            print "search start point from input"
            start = tmp_str[tmp_line][2:]
            endline = tmp_line + 1
            while(tmp_str[endline][:2] == "+_"):
              endline += 1
            end = tmp_str[endline]

            startline = in_line
            while True:
              m.set_seq1(start)
              m.set_seq2(input_str[startline])
              ratio = m.ratio()
              m.set_seq1(end)
              m.set_seq2(input_str[startline+1])
              ratio = (ratio + m.ratio())/2.0
              if(ratio > 0.9):
                break
              startline += 1
            print "startline %d:"%startline, input_str[startline]
            #assertPrint(lines, i, input_str, in_line, tmp_str, tmp_line, output_str)
      else:
        print "ignore:",line
        pass
    elif(command == " "):
      output_str.append(line)
      in_line += 1
      tmp_line += 1
    elif(command == "?"):
        print "ignore",line
    else:
      print "invalid command"
      assertPrint(lines, i, input_str, in_line, tmp_str, tmp_line, output_str)

  writeString(OUTPUT_DIR + filename, "\n".join(output_str))

smartPatch("CMakeList.txt")
smartPatch("main.c")

