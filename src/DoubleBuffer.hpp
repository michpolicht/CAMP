#ifndef DOUBLEBUFFER_HPP_
#define DOUBLEBUFFER_HPP_

#include "DoubleBufferController.hpp"

#include <utility>

/**
 * Double buffer. Double buffer encapsulates two entities of certain data type. The idea
 * is to mark one of them as active and the second one as background. While background
 * data is being overwritten, the active one should remain unchanged. They may be swapped
 * to apply results or, to abandon changes, same entity may be used again for overwrite.
 * Many DoubleBuffer objects may have one controller, thus synchronized swapping can be
 * easily achieved.
 *
 * @see DoubleBufferController.
 */
template <typename T>
class DoubleBuffer
{
	protected:
		T m_buff[2];
		const DoubleBufferController & m_controller;

		DoubleBuffer() {}

	public:
		DoubleBuffer(const DoubleBufferController & controller);

		DoubleBuffer(const DoubleBufferController & controller, const T & initVal);

		DoubleBuffer(const DoubleBufferController & controller, T && initVal);

		DoubleBuffer(const DoubleBufferController & controller, const T & firstVal, const T & secondVal);

		DoubleBuffer(const DoubleBufferController & controller, T && firstVal, T && secondVal);

		template <typename... ARGS>
		decltype(auto) active(ARGS... args) const;

		template <typename... ARGS>
		decltype(auto) active(ARGS... args);

		const T & active() const;

		T & active();

		template <typename... ARGS>
		decltype(auto) background(ARGS... args) const;

		template <typename... ARGS>
		decltype(auto) background(ARGS... args);

		const T & background() const;

		T & background();
};

template <typename T>
DoubleBuffer<T>::DoubleBuffer(const DoubleBufferController & controller):
	m_controller(controller)
{
}

template <typename T>
DoubleBuffer<T>::DoubleBuffer(const DoubleBufferController & controller, const T & initVal):
	m_controller(controller),
	m_buff{initVal, initVal}
{
}

template <typename T>
DoubleBuffer<T>::DoubleBuffer(const DoubleBufferController & controller, T && initVal):
	m_controller(controller),
	m_buff{initVal, std::move(initVal)}
{
}

template <typename T>
DoubleBuffer<T>::DoubleBuffer(const DoubleBufferController & controller, const T & firstVal, const T & secondVal):
	m_controller(controller),
	m_buff{firstVal, secondVal}
{
}

template <typename T>
DoubleBuffer<T>::DoubleBuffer(const DoubleBufferController & controller, T && firstVal, T && secondVal):
	m_controller(controller),
	m_buff{firstVal, secondVal}
{
}

template <typename T>
template <typename... ARGS>
decltype(auto) DoubleBuffer<T>::active(ARGS... args) const {
	return active().operator()(args...);
}

template <typename T>
template <typename... ARGS>
decltype(auto) DoubleBuffer<T>::active(ARGS... args) {
	return active().operator()(args...);
}

template <typename T>
const T & DoubleBuffer<T>::active() const
{
	return m_buff[m_controller.activeBuff()];
}

template <typename T>
T & DoubleBuffer<T>::active()
{
	return m_buff[m_controller.activeBuff()];
}

template <typename T>
const T & DoubleBuffer<T>::background() const
{
	return m_buff[m_controller.backgroundBuff()];
}

template <typename T>
T & DoubleBuffer<T>::background()
{
	return m_buff[m_controller.backgroundBuff()];
}

template <typename T>
template <typename... ARGS>
decltype(auto) DoubleBuffer<T>::background(ARGS... args) const {
	return background().operator()(args...);
}

template <typename T>
template <typename... ARGS>
decltype(auto) DoubleBuffer<T>::background(ARGS... args) {
	return background().operator()(args...);
}

#endif
