import os
#import struct
import sys, getopt
from openpyxl import load_workbook, Workbook
from re import sub, compile
from io import open, StringIO

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
    processMsgDeclarationsStream = StringIO() #appends to headerStream at end
    unionDeclarationStream = StringIO() #appends to headerStream at end

    headerStream.write("#ifndef " + os.path.basename(headerFile) + "_INCLUDE_GUARD\n")
    headerStream.write("#define " + os.path.basename(headerFile) + "_INCLUDE_GUARD\n")
    headerStream.write("#include \"CANHelper.hpp\"\n")
    #headerStream.write("namespace CANHelper\n{\n\tvoid DispatchMsg(can_frame msg);\n}\n")
    headerStream.write("namespace CANHelper::Messages {\n")
    implStream.write('#include \"' + os.path.basename(headerFile) + '.hpp\"\nnamespace CANHelper\n{\n\tvoid CanMsgHandler::DispatchMsg()\n\t{\n\t\tswitch(LATEST_MSG_ID)\n\t\t{\n')

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
        fullName = row[3] + "_" + row[2]

        headerStream.write("#ifdef USE_MSG_" + fullName + "\n")
        headerStream.write("#define CAN_ID_" + fullName + " " + str(id) + "\n")
        headerStream.write("#define CAN_DLC_" + fullName + " " + str(dlc) + "\n")

        structString: str = row[0] + " __attribute__((aligned(8)));\n"
        structString = "\t" + structString.replace("\n", "\n\t")
        structString = structString.replace("_x000D_", "")

        headerStream.write(structString)
        headerStream.write("#endif\n")

        implStream.write("#ifdef USE_MSG_" + fullName + " && (USE_MSG_" + fullName + " & 0b10 == 2)\n")
        implStream.write('\t\tcase ' + id + ':\n')
        implStream.write("\t\t\tCanMsgHandler::processMessage(LATEST_MSG_DATA.as_" + fullName + ");\n")
        implStream.write("\t\t\tbreak;\n")
        implStream.write("#endif\n")

        processMsgDeclarationsStream.write("#ifdef USE_MSG_" + fullName + " && (USE_MSG_" + fullName + " & 0b10 == 2)\n")
        processMsgDeclarationsStream.write("\t\tvoid processMessage(Messages::" + row[3] + "::" + row[2] + "& msg);\n")
        processMsgDeclarationsStream.write("#endif\n")

        unionDeclarationStream.write("#ifdef USE_MSG_" + fullName + " && (USE_MSG_" + fullName + " & 0b01 == 1)\n")
        unionDeclarationStream.write(row[3] + "::" + row[2] + "as_" + fullName + ";")
        unionDeclarationStream.write("#endif\n")

    #process all message declaration
    processMsgDeclarationsStream.write("#ifdef PROCESS_ALL_MSG\n")
    processMsgDeclarationsStream.write("\t\tvoid processAll(Messages::CastedCANPayload& msg);\n")
    processMsgDeclarationsStream.write("#endif\n")

    #add union
    unionDeclarationStream.close()
    headerStream.write("union CastedCANPayload {" + unionDeclarationStream.getvalue() + "};")
    headerStream.write("}\n") #close Messages namespace

    #add helper buffer union and MsgHandler class
    processMsgDeclarationsStream.close()
    headerStream.write(""" namespace CANHelper {
	union CANHelperBuffer { //to try and reduce memory copies (waste of instructions)
		can_frame raw;
		struct {
			char padding[offsetof(can_frame, can_frame.data)]; //aligns payloadBuffer with can_frame.data (without memcpy)
			Messages::CastedCANPayload payloadBuffer;
		};
	};
	
	class CanMsgHandler {
	protected:
		CANHelperBuffer messageRead; //holds the latest received message from the CAN bus
	public:
		CanMsgHandler() = default;""")
    headerStream.write(processMsgDeclarationsStream.getvalue())
    headerStream.write("\t};\n}\n") #close off class and then namespace

    #close header namespace and stream
    headerStream.write("#endif") #close off include guard
    headerStream.close()

    implStream.write("\t\t}\n")
    implStream.write("#ifdef PROCESS_ALL_MSG\n")
    implStream.write("\t\tprocessAll((Messages::CastedCANPayload&) msg);\n")
    implStream.write("#endif\n")
    implStream.write("\t}\n}\n")
    implStream.close()
    print("c++ files generated file generated")

