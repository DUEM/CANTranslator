import os
#import struct
import sys, getopt
from openpyxl import load_workbook, Workbook
from re import sub, compile
from io import open

#handle command line arguments
#inputfile = ''
#outputfile = ''
configFile = ''
headerFile = ''
try:
    opts, args = getopt.getopt(sys.argv[1:],"c:h:",["configTable=", "headerStream="]) #from https://www.tutorialspoint.com/python/python_command_line_arguments.htm.
except:
    print('Translator.py -i <input data> -o <output file> -c <config table file> -h <header file>')
    sys.exit(2)
for opt, arg in opts:
    #print(arg)
    if opt in ("-c", "--configTable"):
        configFile = arg
    elif opt in ("-h", "--headerStream"):
        headerFile = arg
print("Config file: " + configFile)
print("Header file: " + headerFile)

#load configFile
wb = load_workbook(configFile, read_only=True, data_only=True)
sheet = wb.active

if(headerFile != ''):
    if(os.path.isfile(headerFile + '.hpp')):
       os.remove(headerFile + '.hpp')
    if(os.path.isfile(headerFile + '.cpp')):
        os.remove(headerFile + '.cpp')
    headerStream = open(headerFile + '.hpp', 'w')
    implStream = open(headerFile + ".cpp", 'w')

    headerStream.write("#ifndef " + os.path.basename(headerFile) + "_INCLUDE_GUARD\n")
    headerStream.write("#define " + os.path.basename(headerFile) + "_INCLUDE_GUARD\n")
    headerStream.write("#include \"CANHelper.hpp\"\n")
    #headerStream.write("namespace CANHelper\n{\n\tvoid DispatchMsg(can_frame msg);\n}\n")
    headerStream.write("namespace CANHelper::Messages\n{\n")
    implStream.write('#include \"' + os.path.basename(headerFile) + '.hpp\"\nnamespace CANHelper\n{\n\tvoid CanMsgHandler::DispatchMsg(can_frame msg)\n\t{\n\t\tswitch(msg.can_id)\n\t\t{\n')

    rowNumber = 2
    while not (str(sheet[rowNumber][0].value) == "END"):
        row = [cell.value for cell in sheet[rowNumber][19:]]
        id = sheet[rowNumber][4].value
        dlc = sheet[rowNumber][6].value
        rowNumber = rowNumber + 1
        for cell in sheet[rowNumber][:]:
            print(cell.value, end=' ')
        print()
        print("AT 0: " + str(sheet[rowNumber][0].value))

        headerStream.write("#ifdef USE_MSG_" + row[3] + "_" + row[2] + "\n")
        headerStream.write("#define CAN_ID_" + row[3] + "_" + row[2] + " " + str(id) + "\n")
        headerStream.write("#define CAN_DLC_" + row[3] + "_" + row[2] + " " + str(dlc) + "\n")

        headerStream.write("\tnamespace " + row[3] + "\n\t{\n")
        headerStream.write("\t\tstruct _" + row[2] + " : public Messages::CANMsg {\n")
        structString: str = row[0]
        structString = "\t\t\t" + structString.replace("\n", "\n\t\t\t")
        structString = structString.replace("_x000D_", "")
        headerStream.write(structString)

        implStream.write("#ifdef USE_MSG_" + row[3] + "_" + row[2] + "\n")
        implStream.write('\t\tcase ' + id + ':\n')
        implStream.write("\t\t\tMessages::" + "processMessage((Messages::" + row[3] + "::_" + row[2] + "&)msg);\n")
        implStream.write("\t\t\tbreak;\n")

        headerStream.write(" __attribute__((aligned(8)));\n")
        headerStream.write("\t\t\tcanData data;\n")
        headerStream.write("\t\t\t_" + row[2] + "() : CANMsg(" + "CAN_ID_" + row[3] + "_" + row[2] + ", " + "CAN_DLC_" + row[3] + "_" + row[2] + ") { }\n")
        headerStream.write("\t\t};\n\t}\n")
        headerStream.write("\tvoid processMessage(" + row[3] + '::_' + row[2] + "& msg);\n")
        headerStream.write("#endif\n")
        implStream.write("#endif\n")

    #process all message declaration
    headerStream.write("#ifdef PROCESS_ALL_MSG\n")
    headerStream.write("\tvoid processAll(CANMsg& msg);\n")
    headerStream.write("#endif\n")

    headerStream.write("}\n")
    headerStream.write("#endif")
    headerStream.close()

    implStream.write("\t\t}\n")
    implStream.write("#ifdef PROCESS_ALL_MSG\n")
    implStream.write("\t\tprocessAll((Messages::CANMsg&) msg);\n")
    implStream.write("#endif\n")
    implStream.write("\t}\n}\n")
    implStream.close()
    print("c++ files generated file generated")

