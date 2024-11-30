Public Function CamelCase(s As String) As String
    With CreateObject("VBScript.RegExp")
        .Pattern = "[^a-zA-Z]"
        .Global = True
        CamelCase = Replace(StrConv(.Replace(s, " "), vbProperCase), " ", "")
    End With
End Function

Public Function CStruct(structDataType As String, fields As Range, dlc As Integer, Source As String, Item As String) As String ', Optional convertToCamelCase As Boolean = True) As String
    Dim datatypes() As String
    datatypes = Split(structDataType, ",")
    
    'CStruct = "struct canData {"
    'CStruct = "struct " & Source & "::" & Item & " {"
    CStruct = "struct " & Item & " {"
    Dim dataIndex As Integer
    dataIndex = 1
    For i = 0 To UBound(datatypes())
        Dim temp() As String
        temp = Split(Replace(datatypes(i), " ", ""), "*")
        Dim repeats As Integer
        Dim dataType As String
        If UBound(temp()) = 0 Then
            repeats = 1
            dataType = temp(0)
            'MsgBox ("no * | ")
        ElseIf UBound(temp()) = 1 Then
            repeats = temp(0)
            dataType = temp(1)
            'MsgBox ("has repeats specified")
        Else
            CStruct = "ERROR Invalid syntax in " & datatypes(i) & " for calculating repeats"
            Return
        End If
        'MsgBox (createStruct)
        'MsgBox (repeats)
        'MsgBox (dataType)
        Dim cType As String
        Dim noOfBytes As Integer
        cType = Application.WorksheetFunction.VLookup(dataType, Worksheets("DataTypes").Range("A:D"), 2, False)
        noOfBytes = Application.WorksheetFunction.VLookup(dataType, Worksheets("DataTypes").Range("A:D"), 4, False)
        'MsgBox (cType)
        'MsgBox (noOfBytes)
        For j = 1 To repeats
            'MsgBox ("Before " & createStruct)
            Dim fieldName As String
            'If convertToCamelCase Then
            '    fieldName = toCamelCase(fields.Cells(1, dataIndex))
            'Else
                fieldName = fields.Cells(1, dataIndex)
            'End If
            CStruct = CStruct & Chr(13) & Chr(10) & Chr(9) & cType & " " & fieldName & ";"
            'MsgBox (createStruct)
            dataIndex = dataIndex + noOfBytes
            'MsgBox (noOfBytes)
        Next j
    Next i
    'MsgBox (createStruct)
    'MsgBox (dlc)
    'MsgBox (dataIndex)
    If dataIndex <> dlc + 1 Then
        CStruct = "ERROR struct size does not match DLC"
        Return
    End If
    CStruct = CStruct & Chr(13) & Chr(10) & "}"
    'MsgBox (createStruct)
    
End Function

Public Function structUnpackCode(structDataType As String, Optional endian As String = "little")
    Dim datatypes() As String
    datatypes = Split(structDataType, ",")
    
    'Dim structUnpackCode As String
    structUnpackCode = IIf(LCase(endian) = "big", ">", "<")
    For i = 0 To UBound(datatypes())
        Dim temp() As String
        temp = Split(Replace(datatypes(i), " ", ""), "*")
        Dim repeats As Integer
        Dim dataType As String
        If UBound(temp()) = 0 Then
            repeats = 1
            dataType = temp(0)
            'MsgBox ("no * | ")
        ElseIf UBound(temp()) = 1 Then
            repeats = temp(0)
            dataType = temp(1)
            'MsgBox ("has repeats specified")
        Else
            structUnpackCode = "ERROR Invalid syntax in " & datatypes(i) & " for calculating repeats"
            Return
        End If
        
        For j = 1 To repeats
            unpackCode = Application.WorksheetFunction.VLookup(dataType, Worksheets("DataTypes").Range("A:D"), 3, False)
            structUnpackCode = structUnpackCode & unpackCode
        Next j
    Next i
End Function

