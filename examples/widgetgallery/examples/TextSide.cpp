#include <Wt/WLineEdit>
#include <Wt/WTemplate>

SAMPLE_BEGIN(TextSide)
Wt::WTemplate *result = new Wt::WTemplate(Wt::WString::tr("editSide-template"));

Wt::WLineEdit *edit = new Wt::WLineEdit("Username");
edit->setStyleClass("span2");
edit->setId("prependedInput");
result->bindWidget("name", edit);

edit = new Wt::WLineEdit();
edit->setStyleClass("span2");
edit->setId("appendedInput");
result->bindWidget("amount1", edit);

edit = new Wt::WLineEdit();
edit->setStyleClass("span2");
edit->setId("appendedPrependedInput");
result->bindWidget("amount2", edit);

SAMPLE_END(return result)
