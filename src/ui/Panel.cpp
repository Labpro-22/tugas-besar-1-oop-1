#include <algorithm>
#include <string>

#include "ui/Widgets.hpp"

namespace ui {

using ChildEntry =
    std::tuple<std::string, Panel::Layer, std::unique_ptr<Widget>>;

Panel::Panel(sf::Vector2f position, sf::Vector2f size,
             sf::Color backgroundColor)
    : Widget(position, size), style_(backgroundColor), background(size) {}

Panel::Panel(sf::Vector2f position, sf::Vector2f size, const PanelStyle& style)
    : Widget(position, size), style_(style), background(size) {}

Panel::~Panel() = default;

void Panel::addChild(std::unique_ptr<Widget> child) {
  addChild("", std::move(child), Layer::Content);
}

void Panel::addChild(const std::string& key, std::unique_ptr<Widget> child,
                     Layer layer) {
  if (!child) {
    return;
  }

  if (!key.empty()) {
    removeChild(key);
  }

  Widget* childPtr = child.get();
  children_.push_back(std::make_tuple(key, layer, std::move(child)));
  if (!key.empty()) {
    keyedChildren_[key] = childPtr;
  }
}

Widget* Panel::getChild(const std::string& key) {
  const auto it = keyedChildren_.find(key);
  return it == keyedChildren_.end() ? nullptr : it->second;
}

const Widget* Panel::getChild(const std::string& key) const {
  const auto it = keyedChildren_.find(key);
  return it == keyedChildren_.end() ? nullptr : it->second;
}

bool Panel::hasChild(const std::string& key) const {
  return keyedChildren_.find(key) != keyedChildren_.end();
}

bool Panel::removeChild(const std::string& key) {
  if (key.empty()) {
    return false;
  }

  const auto initialSize = children_.size();
  children_.erase(std::remove_if(children_.begin(), children_.end(),
                                 [&](const ChildEntry& entry) {
                                   return std::get<0>(entry) == key;
                                 }),
                  children_.end());

  if (children_.size() == initialSize) {
    return false;
  }

  rebuildKeyIndex();
  return true;
}

void Panel::clearChildren() {
  children_.clear();
  keyedChildren_.clear();
}

void Panel::clearChildren(Layer layer) {
  children_.erase(std::remove_if(children_.begin(), children_.end(),
                                 [&](const ChildEntry& entry) {
                                   return std::get<1>(entry) == layer;
                                 }),
                  children_.end());
  rebuildKeyIndex();
}

void Panel::rebuildKeyIndex() {
  keyedChildren_.clear();
  for (const auto& entry : children_) {
    if (!std::get<0>(entry).empty() && std::get<2>(entry)) {
      keyedChildren_[std::get<0>(entry)] = std::get<2>(entry).get();
    }
  }
}

void Panel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  const Panel::Layer eventOrder[] = {Layer::Overlay, Layer::Content,
                                     Layer::Background};
  for (Panel::Layer layer : eventOrder) {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      if (std::get<1>(*it) != layer || !std::get<2>(*it) ||
          !std::get<2>(*it)->isActive()) {
        continue;
      }
      std::get<2>(*it)->handleEvent(event, window);
    }
  }
}

void Panel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  background.setPosition(position());
  background.setSize(size());
  background.setFillColor(style_.backgroundColor());
  background.setOutlineColor(style_.borderColor());
  background.setOutlineThickness(style_.borderThickness());
  window.draw(background);

  const Panel::Layer renderOrder[] = {Layer::Background, Layer::Content,
                                      Layer::Overlay};
  for (Panel::Layer layer : renderOrder) {
    for (auto& child : children_) {
      if (std::get<1>(child) != layer || !std::get<2>(child) ||
          !std::get<2>(child)->isVisible()) {
        continue;
      }
      std::get<2>(child)->render(window);
    }
  }
}

void Panel::update(sf::RenderWindow& window) {
  const Panel::Layer updateOrder[] = {Layer::Background, Layer::Content,
                                      Layer::Overlay};
  for (Panel::Layer layer : updateOrder) {
    for (auto& child : children_) {
      if (std::get<1>(child) != layer || !std::get<2>(child) ||
          !std::get<2>(child)->isActive()) {
        continue;
      }
      std::get<2>(child)->update(window);
    }
  }
}

}  // namespace ui
