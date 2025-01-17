#include "../includes/LesFenetres_QT.hpp"

#include "../UI/src/Modifier_Equipe_LombricQt_ui.hpp"

Modifier_EquipeQT::Modifier_EquipeQT(int id, MainWindow *parent, Client* cli):
WindowQT(id, parent, client){


  page = new Ui::Modifier_EquipeWidget;
  page->setupUi(this);

  signalMapper->setMapping(page->Return_From_Modif_EquipeToolButton, MAIN_MENU_SCREEN);
  connect(page->Return_From_Modif_EquipeToolButton, SIGNAL(clicked()), signalMapper, SLOT(map()));

  connect(page->Apply_Modif_EquipeToolButton, SIGNAL(clicked()), this, SLOT(ChangeNameLombric()));

}

void Modifier_EquipeQT::initWindow(){
    //parent->setStyleSheet("background-image: url(:/wallpaper/UI/Resources/cropped-1920-1080-521477.jpg);");
    OldLombricName = client->getLombricsName();
    page->Lombric_UnLineEdit->setPlaceholderText(QString(OldLombricName[0].c_str()));
    page->Lombric_DeuxLineEdit->setPlaceholderText(QString(OldLombricName[1].c_str()));
    page->Lombric_TroisLineEdit->setPlaceholderText(QString(OldLombricName[2].c_str()));
    page->Lombric_QuatreLineEdit->setPlaceholderText(QString(OldLombricName[3].c_str()));
    page->Lombric_CinqLineEdit->setPlaceholderText(QString(OldLombricName[4].c_str()));
    page->Lombric_SixLineEdit->setPlaceholderText(QString(OldLombricName[5].c_str()));
    page->Lombric_SeptLineEdit->setPlaceholderText(QString(OldLombricName[6].c_str()));
    page->Lombric_HuitLineEdit->setPlaceholderText(QString(OldLombricName[7].c_str()));

}

void Modifier_EquipeQT::ChangeNameLombric(){
  std::string space = " ";
  std::string index;
  bool isSpace_inLomb;

  std::string lombric1 = page->Lombric_UnLineEdit->text().toStdString();
  std::string lombric2 = page->Lombric_DeuxLineEdit->text().toStdString();
  std::string lombric3 = page->Lombric_TroisLineEdit->text().toStdString();
  std::string lombric4 = page->Lombric_QuatreLineEdit->text().toStdString();
  std::string lombric5 = page->Lombric_CinqLineEdit->text().toStdString();
  std::string lombric6 = page->Lombric_SixLineEdit->text().toStdString();
  std::string lombric7 = page->Lombric_SeptLineEdit->text().toStdString();
  std::string lombric8 = page->Lombric_HuitLineEdit->text().toStdString();

  std::string VecNameLombric[8] = {lombric1,lombric2,lombric3,lombric4,lombric5,lombric6,lombric7,lombric8};

  for (uint32_t nombreLombric = 0; nombreLombric < 8; nombreLombric++){

    isSpace_inLomb = JustSpace_inLomb(VecNameLombric[nombreLombric]);

    if (VecNameLombric[nombreLombric].size() && !isSpace_inLomb)
    {
      client->setLombricName(nombreLombric,VecNameLombric[nombreLombric]);
    }
  }

  page->Lombric_UnLineEdit->clear();
  page->Lombric_DeuxLineEdit->clear();
  page->Lombric_TroisLineEdit->clear();
  page->Lombric_QuatreLineEdit->clear();
  page->Lombric_CinqLineEdit->clear();
  page->Lombric_SixLineEdit->clear();
  page->Lombric_SeptLineEdit->clear();
  page->Lombric_HuitLineEdit->clear();
  OldLombricName = client->getLombricsName();
  page->Lombric_UnLineEdit->setPlaceholderText(QString(OldLombricName[0].c_str()));
  page->Lombric_DeuxLineEdit->setPlaceholderText(QString(OldLombricName[1].c_str()));
  page->Lombric_TroisLineEdit->setPlaceholderText(QString(OldLombricName[2].c_str()));
  page->Lombric_QuatreLineEdit->setPlaceholderText(QString(OldLombricName[3].c_str()));
  page->Lombric_CinqLineEdit->setPlaceholderText(QString(OldLombricName[4].c_str()));
  page->Lombric_SixLineEdit->setPlaceholderText(QString(OldLombricName[5].c_str()));
  page->Lombric_SeptLineEdit->setPlaceholderText(QString(OldLombricName[6].c_str()));
  page->Lombric_HuitLineEdit->setPlaceholderText(QString(OldLombricName[7].c_str()));


}

bool Modifier_EquipeQT::JustSpace_inLomb(std::string text){

  bool isSpace = false;
  text.erase(remove(text.begin(), text.end(), ' '), text.end());
  if (!text.size())
  {
    isSpace = true;
  }
  return isSpace;

}

Modifier_EquipeQT::~Modifier_EquipeQT(){
    delete page;
}
