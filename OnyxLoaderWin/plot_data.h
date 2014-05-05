#ifndef PLOT_DATA_H_
#define PLOT_DATA_H_

#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

using namespace System;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace ZedGraph;

System::Void parse_csv_data(String^ fileName, array<double>^ &data) {
    
    data = gcnew array<double>(0);

    try {
        //Console::WriteLine("trying to open file {0}...", fileName);
        StreamReader^ din = File::OpenText(fileName);

        String^ str;
        int count = 0;
        while ((str = din->ReadLine()) != nullptr) {
            array<String^>^ words;

            String^ delimStr = ",";
            array<Char>^ delimiter = delimStr->ToCharArray();
            words = str->Split(delimiter);
            count++;

            Array::Resize(data, count);
            data[count - 1] = Convert::ToDouble(words[1]);

        }
    }
    catch (Exception^ e) {
        if (dynamic_cast<FileNotFoundException^>(e))
            Console::WriteLine("file '{0}' not found", fileName);
        else
            Console::WriteLine("problem reading file '{0}'", fileName);
    }

}


void create_graph(ZedGraphControl^ zgc, String^ data_file) {

    GraphPane^ myPane = zgc->GraphPane;

    myPane->Title->Text = "Onyx Readings";
    myPane->XAxis->Title->Text = "Time";
    myPane->YAxis->Title->Text = "Intensity, CPM";

    //String^ data_file = "C://Users//Owner//Documents//GitHub//onyxloader//OnyxLoaderWin//test//example_saved_data//test1.csv";
    
    array<double>^ data;
    parse_csv_data(data_file, data);

    array<double>^ index = gcnew array<double>(data->Length);

    for(int i = 0; i<data->Length;i++) {
        index[i] = i;
    }

    LineItem^ myCurve = myPane->AddCurve(data_file, index, data, Color::Red, SymbolType::Diamond);

    // Tell ZedGraph to refigure the
    // axes since the data have changed
    zgc->AxisChange();

    // Retrieve the image
    Image^ img = myPane->GetImage();
    // Save image as PNG

    String^ image_filename = System::IO::Path::Combine(System::IO::Path::GetDirectoryName(data_file), System::IO::Path::GetFileNameWithoutExtension(data_file));
    image_filename += ".png";
    img->Save(image_filename, ImageFormat::Png);
}


#endif //PLOT_DATA_H_