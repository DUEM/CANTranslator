import os
import struct
import sys, getopt
from openpyxl import load_workbook, Workbook
from re import sub
from io import open

#handle command line arguments
inputfile = ''
outputfile = ''
configFile = ''
headerFile = ''
try:
    opts, args = getopt.getopt(sys.argv[1:],"c:i:o:h:",["configTable=", "inputFile=", "outputFile=", "headerStream="]) #from https://www.tutorialspoint.com/python/python_command_line_arguments.htm.
except:
    print('Translator.py -i <input data> -o <output file> -c <config table file> -h <header file>')
    sys.exit(2)
for opt, arg in opts:
    #print(arg)
    if opt in ("-i", "--inputFile"):
        inputfile = arg
    elif opt in ("-o", "--outputFile"):
        outputfile = arg
    elif opt in ("-c", "--configTable"):
        configFile = arg
    elif opt in ("-h", "--headerStream"):
        headerFile = arg
print("Config file: " + configFile)
print("Header file: " + headerFile)

#load configFile
wb = load_workbook(configFile)
sheet = wb.active

#if input file is specified, then translate its contents to something that can be read and processed. It should be a hex dump of CAN messages
if(inputfile != ''):
    if(outputfile == ''):
        print("No ouput location for translated hex dump specified")
        sys.exit(4)
    print("Translating HEX dump")
    
    hexOutBook = Workbook()
    hexOut = hexOutBook.active
    hexOut.title = inputfile.split('\.')[0]

    with open(inputfile) as inputStream:
        rowCounter = 2
        for line in inputStream.readlines()[5:]:
            hexValues = line.split()

            #get message format
            msgId = '0x' + ''.join(e for e in hexValues[1:3])
            print(hexValues[1:3])
            print(msgId)
            configRow = -1
            for i in range(2, sheet.max_row):
                print(sheet["E{}".format(i)].value)
                if(sheet["E{}".format(i)].value == msgId): #assuming all IDs are valid. No need to check if row was not found
                    configRow = i
                    break
            
            #write source and item name
            hexOut["A{}".format(rowCounter)] = sheet["B{}".format(configRow)].value #Item name
            hexOut["B{}".format(rowCounter)] = sheet["C{}".format(configRow)].value #Source of message
            
            #write values
            match sheet["R{}".format(configRow)].value:
                case '2*float32':
                    hexOut["C{}".format(rowCounter)] = struct.unpack('<f', bytes.fromhex(''.join(str(element) for element in line[4:8])))[0] #where < means little endian
                    hexOut["D{}".format(rowCounter)] = struct.unpack('<f', bytes.fromhex(''.join(str(element) for element in line[8:12])))[0]
                case '3*u_int16': #using H for unsigned short (equal to u_int16)
                    hexOut["C{}".format(rowCounter)] = struct.unpack('<H', bytes.fromhex(''.join(str(element) for element in line[4:6])))[0]
                    hexOut["D{}".format(rowCounter)] = struct.unpack('<H', bytes.fromhex(''.join(str(element) for element in line[6:8])))[0]
                    hexOut["E{}".format(rowCounter)] = struct.unpack('<H', bytes.fromhex(''.join(str(element) for element in line[8:10])))[0]
                case 'u_int32 & char[4]':
                    hexOut["C{}".format(rowCounter)] = struct.unpack('<I', bytes.fromhex(''.join(str(element) for element in line[4:8])))[0]
                    hexOut["D{}".format(rowCounter)] = ''.join(struct.unpack('<b', b)[0] for b in line[8:12])
                case _:
                    print("Undefined data format: " + str(sheet["R{}".format(configRow)].value))
                    hexOutBook.save(filename = outputfile + ".xlsx") #save what was successfully translated
                    sys.exit(6)

    hexOutBook.save(filename = outputfile + ".xlsx")
    

#if header file is specified, then generate a header file (and a cpp file) that enables the message formats specified in the config file to be used in C++ code
if(headerFile != ''):
    def toCamelCase(s): #https://www.w3resource.com/python-exercises/string/python-data-type-string-exercise-96.php
        #st = str(s)
        #print(st)
        #s1 = ''.join(c if c != ' ' else '_' for c in s)
        #s2 = ''
        #for char in s1:
            #print((char.isalnum() or "_") and (not ("," or ".")))
        #    if (char.isalnum() or "_") and (not ("," or ".")):
        #        s2 += char
        s1 = sub(r'[^a-zA-Z0-9]+', ' ', s).title().replace(" ", "")
        #s2 = ''.join(d if ((d.isalnum() or '_') and (not (',' or '.'))) else '_' for d in s1)
        return s1
        #s = sub(r"\(\|-\)\+\.,", " ", s).title().replace(" ", "") #r"(_|-)+"
        #return s
        #return ''#.join([s[0].lower(), s[1:]])
        #''.join(x for x in s.title() if not x.isspace())
    if(os.path.isfile(headerFile + '.hpp')):
       os.remove(headerFile + '.hpp')
    if(os.path.isfile(headerFile + '.cpp')):
        os.remove(headerFile + '.cpp')
    headerStream = open(headerFile + '.hpp', 'w')
    implStream = open(headerFile + ".cpp", 'w')
    
    #headerStream.write("#include \"structDefinitions.hpp\"\n#include \"mcp2515.h\"\n")
    headerStream.write("#include \"CANHelper.hpp\"\n")
    #headerStream.write("namespace CANHelper\n{\n\tvoid DispatchMsg(can_frame msg);\n}\n")
    headerStream.write("namespace CANHelper::Messages\n{\n")
    implStream.write('#include \"' + headerFile + '.hpp\"\nnamespace CANHelper\n{\n\tvoid CanMsgHandler::DispatchMsg(can_frame msg)\n\t{\n\t\tswitch(msg.can_id)\n\t\t{\n')

    #create header file with class declarations for each record in the config
    for i in range(4, 24):#sheet.max_row + 1):
        row = [cell.value for cell in sheet[i][0:sheet.max_column]]
        print(row) # list of cell values of this row
        #print(row[1] + "->" + sub(r"(|-)+.", " ", str(row[1])))
        #print(row[1] + '->' + toCamelCase(row[1]))
        #print(row[8] + "->" + toCamelCase(row[8]))
        headerStream.write("#ifdef USE_MSG_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + "\n")
        headerStream.write("#define CAN_ID_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + " " + str(row[4]) + "\n")
        headerStream.write("#define CAN_DLC_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + " " + str(row[6]) + "\n")
        headerStream.write("\tnamespace " + toCamelCase(row[2]) + "\n\t{\n")
        headerStream.write("\t\tstruct _" + toCamelCase(row[1]) + " : public Messages::CANMsg {\n")
        #headerStream.write("\t\t\tMsgMetadata metadata;\n")
        headerStream.write("\t\t\tstruct canData\n\t\t\t{\n")
        #headerStream.write("\t\tclass _" + toCamelCase(row[1]) + " : ")

        implStream.write("#ifdef USE_MSG_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + "\n")
        implStream.write('\t\tcase ' + row[4] + ':\n')
        #implStream.write('\t\t\treturn Messages::' + toCamelCase(row[2]) + '::' + toCamelCase(row[1]) + "(msg.data);\n")
        #implStream.write('\t\t\tMessages::' + toCamelCase(row[2]) + '::_' + toCamelCase(row[1]) + "::processMessage((" + toCamelCase(row[2]) + "::_" + toCamelCase(row[1]) + "&)msg);\n")
        implStream.write("\t\t\tMessages::" + "processMessage((Messages::" + toCamelCase(row[2]) + "::_" + toCamelCase(row[1]) + "&)msg);\n")
        implStream.write("\t\t\tbreak;\n")

        print(row[17])
        match row[17]: #col R is index 17
            case '2*float32':
                headerStream.write("\t\t\t\tfloat " + toCamelCase(row[7]) + ";\n") #col H is index 7
                headerStream.write("\t\t\t\tfloat " + toCamelCase(row[11]) + ";\n") #col L is index 11
            case '3*u_int16':
                headerStream.write("\t\t\t\tuint16_t " + toCamelCase(row[7]) + ";\n")
                headerStream.write("\t\t\t\tuint16_t " + toCamelCase(row[9]) + ";\n") #col J is index 9
                headerStream.write("\t\t\t\tuint16_t " + toCamelCase(row[11]) + ";\n")
            case 'float32, int32':
                headerStream.write("\t\t\t\tfloat " + toCamelCase(row[7]) + ";\n")
                headerStream.write("\t\t\t\tint " + toCamelCase(row[11]) + ";\n")
            case 'u_int32 & char[4]':
                headerStream.write("\t\t\t\tuint32_t " + toCamelCase(row[7]) + ";\n")
                headerStream.write("\t\t\t\tchar* " + toCamelCase(row[11]) + ";\n")
            case 'int16 & 2*u_int16 & 2*u_int8':
                headerStream.write("\t\t\t\tint16_t " + toCamelCase(row[7]) + ";\n")
                headerStream.write("\t\t\t\tuint16_t " + toCamelCase(row[9]) + ";\n")
                headerStream.write("\t\t\t\tuint16_t " + toCamelCase(row[11]) + ";\n")
                headerStream.write("\t\t\t\tuint8_t " + toCamelCase(row[13]) + ";\n") #col N is index 13
                headerStream.write("\t\t\t\tuint8_t " + toCamelCase(row[14]) + ";\n") #col O is index 14
            case '6*int8':
                headerStream.write("\t\t\t\t//WIP\n")
            case _:
                print("Unrecognised data structure ", end='')
                print(row[17])
                headerStream.close()
                sys.exit(3)
        
        #headerStream.write("\t\t\tinline can_id id() { return CAN_ID_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + "; }\n")
        #headerStream.write("\t\t\tinline __u8 dlc() { return CAN_DLC_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + "; }\n")

        headerStream.write("\t\t\t} __attribute__((aligned(8)));\n")
        headerStream.write("\t\t\tcanData data;\n")
        headerStream.write("\t\t\t_" + toCamelCase(row[1]) + "() : CANMsg(" + "CAN_ID_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + ", " + "CAN_DLC_" + toCamelCase(row[2]) + "_" + toCamelCase(row[1]) + ") { }\n")
        headerStream.write("\t\t};\n\t}\n")
        headerStream.write("\tvoid processMessage(" + toCamelCase(row[2]) + '::_' + toCamelCase(row[1]) + "& msg);\n")
        headerStream.write("#endif\n")
        implStream.write("#endif\n")

    headerStream.write("#ifdef PROCESS_ALL_MSG\n")
    headerStream.write("\tvoid processAll(CANMsg& msg);\n")
    headerStream.write("#endif\n")

    headerStream.write("}\n")
    headerStream.close()
    implStream.write("\t\t}\n")
    implStream.write("#ifdef PROCESS_ALL_MSG\n")
    implStream.write("\t\tprocessAll((Messages::CANMsg&) msg);\n")
    implStream.write("#endif\n")
    implStream.write("\t}\n}\n")
    implStream.close()
    print("c++ files generated file generated")

        # match row[17]: #col R is index 17
        #     case '2*float32':
        #         headerStream.write("public MSG_2float32\n\t\t{\n\t\tpublic:\n")
        #         headerStream.write('\t\t\tusing MSG_2float32::MSG_2float32;\n')
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[7]) + "() { return this->get0(); }\n") #col H is index 7
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[11]) + "() { return this->get1(); }\n") #col L is index 11
        #     case '3*u_int16':
        #         headerStream.write("public MSG_3uint16\n\t\t{\n\t\tpublic:\n") #todo class
        #         headerStream.write('\t\t\tusing MSG_3uint16::MSG_3uint16;\n')
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[7]) + "() { return this->get0(); }\n")
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[9]) + "() { return this->get1(); }\n")#col J is index 9
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[11]) + "() { return this->get2(); }\n")
        #     case 'float32, int32':
        #         headerStream.write("public MSG_float32_int32\n\t\t{\n\t\tpublic:\n") #todo class
        #         headerStream.write('\t\t\tusing MSG_float32_int32::MSG_float32_int32;\n')
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[7]) + "() { return this->get0(); }\n")
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[11]) + "() { return this->get1(); }\n")
        #     case 'u_int32 & char[4]':
        #         headerStream.write("public MSG_uint32_char4\n\t\t{\n\t\tpublic:\n") #todo class
        #         headerStream.write('\t\t\tusing MSG_uint32_char4::MSG_uint32_char4;\n')
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[7]) + "() { return this->get0(); }\n")
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[11]) + "() { return this->get1(); }\n")
        #     case 'int16 & 2*u_int16 & 2*u_int8':
        #         headerStream.write("public MSG_int16_2uint16_2uint8\n\t\t{\n\t\tpublic:\n") #todo class
        #         headerStream.write('\t\t\tusing MSG_int16_2uint16_2uint8::MSG_int16_2uint16_2uint8;\n')
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[7]) + "() { return this->get0(); }\n")
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[9]) + "() { return this->get1(); }\n")
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[11]) + "() { return this->get2(); }\n")
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[13]) + "() { return this->get3(); }\n") #col N is index 13
        #         headerStream.write("\t\t\tinline auto " + toCamelCase(row[14]) + "() { return this->get4(); }\n") #col O is index 14
        #     case '6*int8':
        #         headerStream.write("public MSG_6int8\n\t\t{\n\t\tpublic:\n")
        #         headerStream.write('\t\t\tusing MSG_6int8::MSG_6int8\n')
        #         headerStream.write("\t\t\t//WIP\n")
        #     case _:
        #         print("Unrecognised data structure ", end='')
        #         print(row[17])
        #         headerStream.close()
        #         sys.exit(3)
        #headerStream.write("\t\t};\n\t}\n")