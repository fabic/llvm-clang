/** surface/src/main.cpp
 *
 * FabiC.2016-07-06
 *
 * http://www.lucidarme.me/?p=4339
 */

#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>


class HelloWorld : public Gtk::Window
{
public:
  HelloWorld();
  virtual ~HelloWorld();

protected:
  //Signal handlers:
  void on_button_clicked();

  //Member widgets:
  Gtk::Button m_button;
};


HelloWorld::HelloWorld()
  : m_button("Hello World")   // creates a new button with label "Hello World".
{
  // Sets the border width of the window.
  set_border_width(10);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this,
              &HelloWorld::on_button_clicked));

  // This packs the button into the Window (a container).
  add(m_button);

  // The final step is to display this newly created widget...
  m_button.show();
}


HelloWorld::~HelloWorld()
{
}


void HelloWorld::on_button_clicked()
{
  std::cout << "Hello World" << std::endl;
}


/**
 * MAIN !
 */
int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  HelloWorld helloworld;

  //Shows the window and returns when it is closed.
  return app->run(helloworld);
}

