#ifndef DOUBLEBUFFERCONTROLLER_HPP_
#define DOUBLEBUFFERCONTROLLER_HPP_

/**
 * Double buffer controller. Double buffer controller performs swap operation on related
 * double buffers.
 *
 * @see DoubleBuffer.
 *
 * @todo make swapBuffers() truly atomic.
 */
class DoubleBufferController
{
	public:
		typedef bool buff_t;

	public:
		/**
		 * Default constructor. Initializes with active buffer 0.
		 */
		DoubleBufferController(): m_activeBuff(0) {}

		/**
		 * Destructor.
		 */
		virtual ~DoubleBufferController() {}

		/**
		 * Get active buffer.
		 * @return number of buffer which is active (0|1).
		 */
		buff_t activeBuff() const;

		/**
		 * Get background buffer.
		 * @return number of buffer which is background (0|1).
		 */
		buff_t backgroundBuff() const;

		/**
		 * Swap buffers.
		 */
		void swapBuffers();

	protected:
		buff_t m_activeBuff;
};

inline
DoubleBufferController::buff_t DoubleBufferController::activeBuff() const
{
	return m_activeBuff;
}

inline
DoubleBufferController::buff_t DoubleBufferController::backgroundBuff() const
{
	return m_activeBuff ^ 0x1;
}

inline
void DoubleBufferController::swapBuffers()
{
	m_activeBuff ^= 0x1;
}

#endif
