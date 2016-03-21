#include "CSDDictionnary.h"
//#include<CData.h>
#include<InNumber.h>
#include<InFile.h>
#include<ControlBrowseDirectory.h>
#include<Copy.h>
#include<CGInNumber.h>
#include<CSDOperatorMerge.h>
#include<cimage.h>
#include<CControlRessource.h>
//#include <CGInNumber.h>
#include <divint.h>

#include "uid.h"
#include <ViewImage.h>
#include <InFile.h>
#include <DataBoolean.h>
#include <DataDateTime.h>
#include <DataMap.h>
#include <DataNumber.h>
#include <DataStack.h>
#include <DataString.h>
#include <DataTable.h>
#include <DataVector.h>
#include <DataImageQt.h>
#include <DataDictionnary.h>
#include <and.h>
#include <inverse.h>
#include <or.h>
#include <OperatorLoadBoolean.h>
#include <OperatorSaveBoolean.h>
#include <Copy.h>
#include <CSDOperatorMerge.h>
#include <current.h>
#include <diff.h>
#include <getday.h>
#include <getmonth.h>
#include <gettime.h>
#include <getyear.h>
#include <msectostring.h>
#include <exists.h>
#include <isdir.h>
#include <isfile.h>
#include <readdir.h>
#include <readfile.h>
#include <OperatorBaseNameString.h>
#include <OperatorSuffixString.h>

#include <editorstringtext2.h>

#include <async.h>
#include <cast.h>
#include <counter.h>
#include <fromstring.h>
#include <gettype.h>
#include <id.h>
#include <if.h>
//#include <releasenewstate.h>
#include <merge.h>
#include <sync.h>
#include <SCDOperatorSync3Generic.h>
#include <blank.h>
#include <DataByAddressToDataByFile.h>
#include <DataByFileToDataByAddress.h>
#include <OperatorGenerateRandomFileData.h>
#include <SCDOperatorLoadByFiledGeneric.h>
#include <tostring.h>
#include <InNumber.h>


#include <insert.h>
#include <keys.h>
#include <remove.h>
#include <OperatorSaveMap.h>
#include <OperatorLoadMap.h>


#include <value.h>
#include <add.h>
#include <diffnumber.h>
#include <div.h>
#include <equal.h>
#include <inf.h>
#include <mult.h>
#include <sup.h>
#include <NumberAddition.h>
#include <OperatorMaxNumber.h>
#include <OperatorMinNumber.h>
#include <OperatorPowNumber.h>
#include <OperatorSaveNumber.h>
#include <absnumber.h>



#include <concat.h>
#include <regexp.h>
#include <replace.h>
#include <split.h>
#include <instring.h>
#include<OperatorSaveString.h>
#include<OperatorInsertString.h>
#include<OperatorSizeString.h>


#include <getcolumnname.h>
#include <getsize.h>
#include <gettableelement.h>
#include <resizetable.h>
#include <setcolumnname.h>
#include <settableelement.h>
#include <OperatorGetColTable.h>
#include <OperatorGetRowTable.h>
#include <OperatorLoadTable.h>
#include <OperatorRmColTable.h>
#include <OperatorRmRowTable.h>
#include <OperatorSaveTable.h>
#include <OperatorSetColTable.h>
#include <OperatorSetRowTable.h>



#include <get.h>
#include <set.h>
#include <size.h>
#include <resize.h>
#include <OperatorSortVector.h>
#include <OperatorIteratorVector.h>
#include <OperatorIteratorOutVector.h>
#include <OperatorPushBackVector.h>
#include <erase.h>
#include<OperatorLoadVector.h>
#include<OperatorSaveVector.h>
#include <Boolean/ViewBoolean.h>
#include <Boolean/EditorBoolean.h>
#include <ViewNumber.h>
#include <EditorNumber.h>
#include <SliderNumber.h>
#include <slidernumber2.h>
#include <compare.h>
#include <EditorString.h>
#include <ViewString.h>
#include <EditorText.h>
#include "Boolean/Button.h"
#include <EditorDate.h>
#include <EditorDateTime.h>
#include <EditorTime.h>
#include <SelectorFile.h>

#include <ListBoxVector.h>
#include <EditorVector.h>
#include <ViewVector.h>
#include <SelectorVector.h>
#include <locker.h>
#include <EditorMap.h>
#include <DrawImage.h>
#include <MarkerImage.h>
#include <ViewMap.h>
#include <AlphaLayer.h>
#include <Curves.h>
#include <EditorTable.h>
#include <ViewTable.h>
#include <LoadImageQT.h>
#include <controlprogressbar.h>
#include <numberfromstring.h>
#include <numbertostring.h>
#include<SCDOperatorGetDataKeysVector.h>
#include<CSDOperatorHideShowInterface.h>
#include<ControlEditorArgument.h>
#include<ChangeLayout.h>
#include<CSDOperatorProjectPath.h>
#include<OperatorProjectTmpPathString.h>
#include<filedir.h>
#include<copyfile.h>
#include<copydir.h>
#include <rmdir.h>
#include <rmfile.h>
#include <viewline.h>
#include <CLogger.h>
#include <editorstringwithinput.h>
#include <asymmetric.h>
#include <exitloop.h>
#include <loop.h>
#include "loopteleport.h"
#include "endloopteleport.h"
#include <priority.h>
#include "relativedir.h"
#include "OperatorExitInstance.h"
#include "OperatorGetMode.h"
#include "OperatorTableToCSV.h"
#include "OperatorCSVToTable.h"
#include "OperatorBreakPoint.h"
#include "makedir.h"
#include "OperatorStop.h"
#include "OperatorPlay.h"
#include "COperatorAssert.h"
#include "cli.h"
#include "clisync.h"
//#include "OperatorErrorString.h"
CSDDictionnary::CSDDictionnary(){
    this->setNameDictionnary("CSD");
    this->setVersion("2.0.0");
    this->setInformation("The CSD dictionary implements elementary data structures and operators.");
    CLogger::getInstance()->registerComponent("SCD");
    CLogger::getInstance()->log("SCD",CLogger::INFO,"Init SCD");
}

void CSDDictionnary::collectData(){
    this->registerData(new DataNumber);
    this->registerData(new DataString);
    this->registerData(new DataBoolean);
    this->registerData(new DataGeneric);
    this->registerData(new DataDateTime);
    this->registerData(new DataMap);
    //    this->registerData(new DataStack);
    this->registerData(new DataTable);
    this->registerData(new DataVector);
    this->registerData(new DataImageQt);
    this->registerData(new DataDictionnary);
    //collectData()_end
}

void CSDDictionnary::collectOperator(){

    this->registerOperator(new Copy);

    //Boolean
    this->registerOperator(new And);
    this->registerOperator(new inverse);
    this->registerOperator(new Or);
    this->registerOperator(new OperatorLoadBoolean);
    this->registerOperator(new OperatorSaveBoolean);

    //Number
    this->registerOperator(new add);
    this->registerOperator(new diffnumber);
    this->registerOperator(new numberdiv);
    this->registerOperator(new numberdivint);
    this->registerOperator(new equalnumber);
    this->registerOperator(new inf);
    this->registerOperator(new mult);
    this->registerOperator(new sup);
    this->registerOperator(new OperatorMaxNumber);
    this->registerOperator(new OperatorMinNumber);
    this->registerOperator(new OperatorAbsNumber);
    this->registerOperator(new OperatorPowNumber);
    this->registerOperator(new NumberFromString);
    this->registerOperator(new NumberToString);
    this->registerOperator(new innumber);
    this->registerOperator(new OperatorSaveNumber);
    this->registerOperator(new uid);

    //String
    this->registerOperator(new concat);
    this->registerOperator(new regexp);
    this->registerOperator(new OperatorReplaceGeneric);
    this->registerOperator(new split);
    this->registerOperator(new compare);
    this->registerOperator(new OperatorInsertString);
    this->registerOperator(new OperatorSizeString);

    //VECTOR
    this->registerOperator(new get);
    this->registerOperator(new set);
    this->registerOperator(new size);
    this->registerOperator(new erase);
    this->registerOperator(new OperatorSortVector);
    this->registerOperator(new OperatorIteratorVector);
    this->registerOperator(new OperatorIteratorOutVector);
    this->registerOperator(new OperatorPushBackVector);
    this->registerOperator(new resize);
    this->registerOperator(new OperatorSaveVector);
    this->registerOperator(new OperatorLoadVector);

    //TABLE
    this->registerOperator(new getSizeTable);
    this->registerOperator(new ResizeTable);
    this->registerOperator(new getTableElement);
    this->registerOperator(new setTableElement);
    this->registerOperator(new OperatorGetColTable);
    this->registerOperator(new OperatorGetRowTable);
    this->registerOperator(new OperatorRmColTable);
    this->registerOperator(new OperatorRmRowTable);
//    this->registerOperator(new OperatorSetColTable);
//    this->registerOperator(new OperatorSetRowTable);
    this->registerOperator(new OperatorCSVToTable);
    this->registerOperator(new OperatorTableToCSV);
    this->registerOperator(new OperatorLoadTable);
    this->registerOperator(new OperatorSaveTable);

    //map
    this->registerOperator(new value);
    this->registerOperator(new insert);
    this->registerOperator(new keys);
    this->registerOperator(new removemapelement);
    this->registerOperator(new OperatorSaveMap);
    this->registerOperator(new OperatorLoadMap);

    //Generic
//    this->registerOperator(new fromstring);
//    this->registerOperator(new tostring);
    this->registerOperator(new blank);
//    this->registerOperator(new gettype);
//    this->registerOperator(new DataByAddressToDataByFile);
//    this->registerOperator(new DataByFileToDataByAddress);
//    this->registerOperator(new SCDOperatorLoadByFiledGeneric);

    //Control-flow
    this->registerOperator(new id);
    this->registerOperator(new If);
    this->registerOperator(new OperatorMergeGeneric);
    this->registerOperator(new syncGen);
    this->registerOperator(new async);
    this->registerOperator(new locker);
    this->registerOperator(new exitloop);
    this->registerOperator(new counter);
    this->registerOperator(new loop);
    this->registerOperator(new forteleport);
    this->registerOperator(new endforteleport);
//        this->registerOperator(new releasenewstate);
//    this->registerOperator(new asymmetric);
//   this->registerOperator(new priority);

    //time
//    this->registerOperator(new current);
//    this->registerOperator(new diff);
//    this->registerOperator(new getday);
//    this->registerOperator(new getmonth);
//    this->registerOperator(new gettime);
//    this->registerOperator(new getyear);
//    this->registerOperator(new msectostring);


    //File system
    this->registerOperator(new exists);
    this->registerOperator(new isDir);
    this->registerOperator(new isFile);
    this->registerOperator(new readdir);
    this->registerOperator(new readfile);
    this->registerOperator(new filedir);
//    this->registerOperator(new relativedir);
    this->registerOperator(new OperatorBaseNameString);
    this->registerOperator(new OperatorSuffixString);
    this->registerOperator(new copydir);
    this->registerOperator(new copyfile);
    this->registerOperator(new rmDir);
    this->registerOperator(new rmfile);
    this->registerOperator(new MakeDir);
    this->registerOperator(new instring);
    this->registerOperator(new OperatorSaveString);

    this->registerOperator(new LoadImageQT);
    //Introspection
//    this->registerOperator(new SCDOperatorGetDataKeysVector);
//    this->registerOperator(new CSDOperatorHideShowInterface);
    this->registerOperator(new CSDOperatorProjectPath);
//    this->registerOperator(new OperatorProjectTmpPath);
    this->registerOperator(new OperatorExitInstance);
//    this->registerOperator(new GetMode);
    this->registerOperator(new OperatorBreakPoint);
//    this->registerOperator(new OperatorPlay);
//    this->registerOperator(new OperatorStop);
//    this->registerOperator(new OperatorErrorString);
    this->registerOperator(new COperatorAssert);

//system
    this->registerOperator(new CommandLineSync);
    this->registerOperator(new CommandLine);

    //collectOperator()_end
}
#include "BackgroundControl.h"
#include "EditorCheckBox.h"
#include "ViewMenu.h"
#include "ColorSurface.h"
void CSDDictionnary::collectControl(){

    this->registerControl(new InFile);
    this->registerControl(new OutFile);
    this->registerControl(new ControlBrowseDirectory);
    this->registerControl(new ViewImage);
    this->registerControl(new DrawImage);

    this->registerControl(new ViewBoolean);
    this->registerControl(new EditorBoolean);
    this->registerControl(new EditorNumber);
    this->registerControl(new SliderNumber);
    this->registerControl(new slidernumber2);
    this->registerControl(new ViewNumber);
    this->registerControl(new ViewString);
    this->registerControl(new EditorString);
    this->registerControl(new EditorText);
    this->registerControl(new editorstringtext2);
    this->registerControl(new EditorButton);
//    this->registerControl(new EditorDate);
//    this->registerControl(new EditorTime);
//    this->registerControl(new EditorDateTime);
    this->registerControl(new SelectorFile);
    this->registerControl(new ListBoxVector);
    this->registerControl(new EditorVector);
    this->registerControl(new ViewVector);
//    this->registerControl(new SelectorVector);
    this->registerControl(new ViewMap);
    this->registerControl(new EditorMap);

    this->registerControl(new MarkerImage);
    //    this->registerControl(new Curves);
    this->registerControl(new EditorTable);
    this->registerControl(new ViewTable);
    this->registerControl(new ControlProgressBar);
//    this->registerControl(new ChangeLayout);
    this->registerControl(new ControlEditorArgument);
    this->registerControl(new ViewLine);
    this->registerControl(new EditorStringWithInput);
//    this->registerControl(new AlphaLayer);
//    this->registerControl(new ColorSurface);
    this->registerControl(new ViewMenu);
    this->registerControl(new BackgroundControl);
    this->registerControl(new EditorCheckBox);
//    this->registerControl(new CControlRessource);
//    this->registerControl(new CControlRessourceProject);
    //collectControl()_end
}
