rate = 2.54

def writeToFile(z, xy):
    detail = xy.split('Y')
    # print(lines, detail, detail[0][1:len(detail[0])])
    x = float(detail[0][1:len(detail[0])]) * rate * 1000
    y = float(detail[1]) * rate * 1000
    #out = 'Z' + str(z) + ',' + str(x) + ',' + str(y)
    out = "Z%.0f,%.0f,%.0f;" %(x, y, z)
    # print(out)
    return out

def gcodeInterpretor(inputFile, outputFile, travalSpeed, cuttingSpeed, upperZ, lowerZ):
    lastCmd = ''
    preZ = upperZ
    for lines in inputFile:
        strs = lines.split()
        if(len(strs) == 2):         # Commands with two parts
            if(strs[0] == 'G00'):   # Traval
                z_height = upperZ
                if(preZ == upperZ): # Set traval speed
                    outputFile.write('V' +str(travalSpeed)+'\n')
            elif(strs[0] == 'G01'): # Cut
                z_height = lowerZ
                if(preZ == upperZ): # Set cut speed
                    outputFile.write('V' +str(cuttingSpeed)+'\n')
            preZ = z_height
            if(strs[1][0]=='Z'):
                value = lastCmd
            elif(strs[1][0]=='X'):
                value = strs[1]
                lastCmd = strs[1]
            else:
                pass
                # print(strs[1][0])
            if(value != ''):
                out = writeToFile(z_height, value)
                outputFile.write(out+'\n')

def main():
    top_layer_filename = 'top.gcode'
    drill_filename = 'drill.gcode'
    boarder_filename = ['boarder.gcode', 'boarder2.gcode']
    output_filename = 'output.gcode'
    '''Configuration'''
    # Speed
    spindle_rpm = 10000
    traval_speed = 80
    top_layer_cutting_speed = 10
    drill_cutting_speed = 5
    boarder_cutting_speed = [5, 5]
    # Height
    traval_height = 100
    top_layer_cutting_height = -15
    drill_cutting_height = -200
    boarder_cutting_height = [-50, -200]
    '''Configuration'''
    output_file = open(output_filename, 'w')
    top_layer = open(top_layer_filename)
    prefix = (";;^IN;\n!MC0;\nV125.0;\n^PR;\nZ0,0,1000;\n^PA;\n!RC%d;!MC1;Z0,0,100;V10.0;\n" %(spindle_rpm))
    posfix = "!MC0;Z0,0,1000;"

    output_file.write(prefix)
    # Top Layer
    top_layer_file = open(top_layer_filename)
    gcodeInterpretor(top_layer_file, output_file, traval_speed, top_layer_cutting_speed, 
                     traval_height, top_layer_cutting_height)
    # Drill
    drill_file = open(drill_filename)
    gcodeInterpretor(drill_file, output_file, traval_speed, drill_cutting_speed, 
                     traval_height, drill_cutting_height)
    # Boarder
    for i in range(len(boarder_filename)):
        boarder_file = open(boarder_filename[i])
        gcodeInterpretor(boarder_file, output_file, traval_speed, boarder_cutting_speed, 
                         traval_height, boarder_cutting_height[i])
    print("Translate suscced!")
    
if __name__ == "__main__":
    main()