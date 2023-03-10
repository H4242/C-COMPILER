
// Generated from c:\Users\saadg\Desktop\PLD-COMP\compiler\ifcc.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({ "all", "warnings", "unchecked", "unused", "cast" })
public class ifccLexer extends Lexer {
	static {
		RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION);
	}

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache = new PredictionContextCache();
	public static final int T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, RETURN = 8, CONST = 9,
			COMMENT = 10, EOLCOMMENT = 11, DIRECTIVE = 12, WS = 13;
	public static String[] channelNames = {
			"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
			"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
				"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "RETURN", "CONST",
				"COMMENT", "EOLCOMMENT", "DIRECTIVE", "WS"
		};
	}

	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
				null, "'int'", "'main'", "'('", "')'", "'{'", "';'", "'}'", "'return'"
		};
	}

	private static final String[] _LITERAL_NAMES = makeLiteralNames();

	private static String[] makeSymbolicNames() {
		return new String[] {
				null, null, null, null, null, null, null, null, "RETURN", "CONST", "COMMENT",
				"EOLCOMMENT", "DIRECTIVE", "WS"
		};
	}

	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	public ifccLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this, _ATN, _decisionToDFA, _sharedContextCache);
	}

	@Override
	public String getGrammarFileName() {
		return "ifcc.g4";
	}

	@Override
	public String[] getRuleNames() {
		return ruleNames;
	}

	@Override
	public String getSerializedATN() {
		return _serializedATN;
	}

	@Override
	public String[] getChannelNames() {
		return channelNames;
	}

	@Override
	public String[] getModeNames() {
		return modeNames;
	}

	@Override
	public ATN getATN() {
		return _ATN;
	}

	public static final String _serializedATN = "\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\17f\b\1\4\2\t\2\4"
			+
			"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t" +
			"\13\4\f\t\f\4\r\t\r\4\16\t\16\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\4" +
			"\3\4\3\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\n\6" +
			"\n9\n\n\r\n\16\n:\3\13\3\13\3\13\3\13\7\13A\n\13\f\13\16\13D\13\13\3\13" +
			"\3\13\3\13\3\13\3\13\3\f\3\f\3\f\3\f\7\fO\n\f\f\f\16\fR\13\f\3\f\3\f\3" +
			"\f\3\f\3\r\3\r\7\rZ\n\r\f\r\16\r]\13\r\3\r\3\r\3\r\3\r\3\16\3\16\3\16" +
			"\3\16\5BP[\2\17\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31" +
			"\16\33\17\3\2\4\3\2\62;\5\2\13\f\17\17\"\"\2i\2\3\3\2\2\2\2\5\3\2\2\2" +
			"\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3" +
			"\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2" +
			"\2\3\35\3\2\2\2\5!\3\2\2\2\7&\3\2\2\2\t(\3\2\2\2\13*\3\2\2\2\r,\3\2\2" +
			"\2\17.\3\2\2\2\21\60\3\2\2\2\238\3\2\2\2\25<\3\2\2\2\27J\3\2\2\2\31W\3" +
			"\2\2\2\33b\3\2\2\2\35\36\7k\2\2\36\37\7p\2\2\37 \7v\2\2 \4\3\2\2\2!\"" +
			"\7o\2\2\"#\7c\2\2#$\7k\2\2$%\7p\2\2%\6\3\2\2\2&\'\7*\2\2\'\b\3\2\2\2(" +
			")\7+\2\2)\n\3\2\2\2*+\7}\2\2+\f\3\2\2\2,-\7=\2\2-\16\3\2\2\2./\7\177\2" +
			"\2/\20\3\2\2\2\60\61\7t\2\2\61\62\7g\2\2\62\63\7v\2\2\63\64\7w\2\2\64" +
			"\65\7t\2\2\65\66\7p\2\2\66\22\3\2\2\2\679\t\2\2\28\67\3\2\2\29:\3\2\2" +
			"\2:8\3\2\2\2:;\3\2\2\2;\24\3\2\2\2<=\7\61\2\2=>\7,\2\2>B\3\2\2\2?A\13" +
			"\2\2\2@?\3\2\2\2AD\3\2\2\2BC\3\2\2\2B@\3\2\2\2CE\3\2\2\2DB\3\2\2\2EF\7" +
			",\2\2FG\7\61\2\2GH\3\2\2\2HI\b\13\2\2I\26\3\2\2\2JK\7\61\2\2KL\7\61\2" +
			"\2LP\3\2\2\2MO\13\2\2\2NM\3\2\2\2OR\3\2\2\2PQ\3\2\2\2PN\3\2\2\2QS\3\2" +
			"\2\2RP\3\2\2\2ST\7\f\2\2TU\3\2\2\2UV\b\f\2\2V\30\3\2\2\2W[\7%\2\2XZ\13" +
			"\2\2\2YX\3\2\2\2Z]\3\2\2\2[\\\3\2\2\2[Y\3\2\2\2\\^\3\2\2\2][\3\2\2\2^" +
			"_\7\f\2\2_`\3\2\2\2`a\b\r\2\2a\32\3\2\2\2bc\t\3\2\2cd\3\2\2\2de\b\16\3" +
			"\2e\34\3\2\2\2\7\2:BP[\4\b\2\2\2\3\2";
	public static final ATN _ATN = new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}