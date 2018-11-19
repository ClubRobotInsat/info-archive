/// Le code suivant permets de tester qu'un donne à ElecCommunicator un objet qui peut lire et écrire des trames
/// Voir la [réponse sur StackOverflow](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature)

#include <optional>
#include <type_traits>

#include <communication/GlobalFrame.h>

/**
 * Cette structure permets de s'assurer que l'`ElecCommunicator` ait en argument template une classe
 * une classe qui sache lire et écrire des `GlobalFrame` en proposant les méthodes suivantes :
 *
 * class ParsingClass {
 * public:
 *      void read_frame(const GlobalFrame&);
 *      // La classe peut retourner une trame si elle le veut, ou rien
 *      std::optional<GlobalFrame> write_frame() const;
 * }
 */

namespace Communication {
	namespace detail {
		// Si la structure est instantiée, une erreur est levée
		template <typename, typename T>
		struct read_frames {
			static_assert(std::integral_constant<T, false>::value, "Second template parameter needs to be of function type.");
		};

		// Specialisation qui test que 'C' possède la méthode 'read_frame'
		// dont le type de retour et les arguments sont donnés par `Ret(Args...)`
		template <typename C, typename Ret, typename... Args>
		struct read_frames<C, Ret(Args...)> {
		private:
			template <typename T>
			static constexpr auto check(T*) ->
			    typename std::is_same<decltype(std::declval<T>().read_frame(std::declval<Args>()...)), Ret>::type;

			template <typename>
			static constexpr std::false_type check(...);

			typedef decltype(check<C>(0)) type;

		public:
			// on sait dès la compilation si C possède la méthode 'read_frame'
			static constexpr bool value = type::value;
		};

		// Même chose, mais pour la méthode 'write_frame'
		template <typename, typename T>
		struct write_frames {
			static_assert(std::integral_constant<T, false>::value, "Second template parameter needs to be of function type.");
		};
		template <typename C, typename Ret, typename... Args>
		struct write_frames<C, Ret(Args...)> {
		private:
			template <typename T>
			static constexpr auto check(T*) ->
			    typename std::is_same<decltype(std::declval<T>().write_frame(std::declval<Args>()...)), Ret>::type;

			template <typename>
			static constexpr std::false_type check(...);

			typedef decltype(check<C>(0)) type;

		public:
			static constexpr bool value = type::value;
		};

	} // namespace detail

	// Finalement, cette interface teste que 'T' sache lire et écrire des `GlobalFrame`s
	template <typename T>
	struct parses_frames {
		static constexpr bool value = detail::read_frames<T, void(const GlobalFrame&)>::value &&
		                              detail::write_frames<T, std::optional<GlobalFrame>(void)>::value;
	};
} // namespace Communication