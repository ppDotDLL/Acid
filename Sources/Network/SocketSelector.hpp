#pragma once

#include <memory>
#include "Engine/Exports.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	class Socket;

	/// <summary>
	/// Socket selectors provide a way to wait until some data is available on a set of sockets,
	/// instead of just one. This is convenient when you have multiple sockets that may
	/// possibly receive data, but you don't know which one will be ready first.
	/// In particular, it avoids to use a thread for each socket; with selectors,
	/// a single thread can handle all the sockets.
	///
	/// All types of sockets can be used in a selector:
	/// \li acid::TcpListener
	/// \li acid::TcpSocket
	/// \li acid::UdpSocket
	///
	/// A selector doesn't store its own copies of the sockets (socket classes are not copyable anyway),
	/// it simply keeps a reference to the original sockets that you pass to the "add" function.
	/// Therefore, you can't use the selector as a socket container, you must store them outside and make sure
	/// that they are alive as long as they are used in the selector.
	///
	/// Using a selector is simple:
	/// \li populate the selector with all the sockets that you want to observe
	/// \li make it wait until there is data available on any of the sockets
	/// \li test each socket to find out which ones are ready
	/// </summary>
	class ACID_EXPORT SocketSelector
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		SocketSelector();

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="copy"> Instance to copy. </param>
		SocketSelector(const SocketSelector &copy);

		/// <summary>
		/// Add a new socket to the selector.
		///
		/// This function keeps a weak reference to the socket, so you have to make sure
		/// that the socket is not destroyed while it is stored in the selector.
		/// This function does nothing if the socket is not valid.
		/// </summary>
		/// <param name="socket"> Reference to the socket to add. </param>
		void Add(Socket &socket);

		/// <summary>
		/// Remove a socket from the selector.
		/// This function doesn't destroy the socket, it simply removes the reference that the selector has to it.
		/// </summary>
		/// <param name="socket"> Reference to the socket to remove. </param>
		void Remove(Socket &socket);

		/// <summary>
		/// Remove all the sockets stored in the selector.
		/// This function doesn't destroy any instance, it simply removes all the references that
		/// the selector has to external sockets.
		/// </summary>
		void Clear();

		/// <summary>
		/// Wait until one or more sockets are ready to receive.
		/// This function returns as soon as at least one socket has some data available to be received.
		/// To know which sockets are ready, use the isReady function.
		/// If you use a timeout and no socket is ready before the timeout is over, the function returns false.
		/// </summary>
		/// <param name="timeout"> Maximum time to wait, (use Time::Zero for infinity). </param>
		/// <returns> True if there are sockets ready, false otherwise. </returns>
		bool Wait(Time timeout = Time::Zero);

		/// <summary>
		/// Test a socket to know if it is ready to receive data.
		///
		/// This function must be used after a call to Wait, to know which sockets are ready to receive data.
		/// If a socket is ready, a call to receive will never block because we know that there is data available to read.
		/// Note that if this function returns true for a TcpListener, this means that it is ready to accept a new connection.
		/// </summary>
		/// <param name="socket"> Socket to test. </param>
		/// <returns> True if the socket is ready to read, false otherwise. </returns>
		bool IsReady(const Socket &socket) const;

		/// <summary>
		/// Overload of assignment operator.
		/// </summary>
		/// <param name="right"> Instance to assign. </param>
		/// <returns> Reference to self. </returns>
		SocketSelector &operator=(const SocketSelector &right);
	private:
		struct SocketSelectorImpl;

		/// Opaque pointer to the implementation (which requires OS-specific types).
		std::unique_ptr<SocketSelectorImpl> m_impl;
	};
}
