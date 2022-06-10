////#include <concepts>
//#include <experimental\coroutine>
//#include <exception>
//#include <iostream>
//
//struct ReturnObject {
//	struct promise_type {
//		ReturnObject get_return_object() { return {}; }
//		std::experimental::suspend_never initial_suspend() { return {}; }
//		std::experimental::suspend_never final_suspend() noexcept { return {}; }
//		void unhandled_exception() {}
//	};
//};
//
//struct Awaiter {
//	std::experimental::coroutine_handle<> *hp_;
//	constexpr bool await_ready() const noexcept { return false; }
//	void await_suspend(std::experimental::coroutine_handle<> h) { *hp_ = h; }
//	constexpr void await_resume() const noexcept {}
//};
//
//ReturnObject
//counter(std::experimental::coroutine_handle<> *continuation_out)
//{
//	Awaiter a{ continuation_out };
//	for (unsigned i = 0;; ++i) {
//		co_await a;
//		std::cout << "counter: " << i << std::endl;
//	}
//}
//
//void
//main1()
//{
//	std::experimental::coroutine_handle<> h;
//	counter(&h);
//	for (int i = 0; i < 3; ++i) {
//		std::cout << "In main1 function\n";
//		h();
//	}
//	h.destroy();
//}