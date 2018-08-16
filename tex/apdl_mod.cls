\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{apdl_mod}[2018/08/02 ANSYS APDL]

\LoadClass[12pt]{article}

\RequirePackage[utf8]{inputenc}
\RequirePackage[T1]{fontenc}
\RequirePackage{newpxtext,newpxmath}
\RequirePackage[magyar]{babel}
\RequirePackage{indentfirst}

\DeclareOption{magyar}{
  \def\HeaderDept{BME Műszaki Mechanikai Tanszék}
  \def\HeaderDate{Utolsó frissítés}
  \def\HeaderCourse{Tantárgy}
  \def\HeaderTask{Feladat száma}
  \def\HeaderDetail{Részletességi szint}
}

\DeclareOption{english}{
  \def\HeaderDept{BME Department of Applied Mechanics}
  \def\HeaderDate{Last update}
  \def\HeaderCourse{Course}
  \def\HeaderTask{Number of task}
  \def\HeaderDetail{Detail level}
}

\ExecuteOptions{magyar}
\ProcessOptions\relax

\RequirePackage{geometry}
\geometry{
	a4paper,
	left=1.5cm, 
	right=1.5cm, 
	top=1.5cm, 
	bottom=1.5cm
}

\newcommand{\course}[1]{\gdef\@course{#1}}
\course{}
\newcommand{\tasknum}[1]{\gdef\@tasknum{#1}}
\tasknum{}
% Default detail levels
\def\detailPDF{2}
\def\detailAPDL{2}
% Command for user override
\newcommand{\detailLevelPDF}[1]{\def\detailPDF{#1}}
\newcommand{\detailLevelAPDL}[1]{\def\detailAPDL{#1}}

\providetoggle{exportcode}
\settoggle{exportcode}{false}

\newcommand{\codefile}[1]{
	\newwrite\file
	\immediate\openout\file=#1
	\settoggle{exportcode}{true}
}

\renewcommand{\maketitle}{%
    {\huge\fontseries{b}\selectfont
    \noindent\@title}
    \medskip \\ 
    \HeaderDept \hfill \HeaderTask: \@tasknum \\
    \HeaderCourse: \@course \hfill \HeaderDate: \@date \\
    \HeaderDetail: \detailPDF
    \par\bigskip
    \iftoggle{exportcode} 
    {
	    \immediate\write\file{!!!\@title}
		\immediate\write\file{!\HeaderDept}
		\immediate\write\file{!\HeaderCourse: \@course }
		\immediate\write\file{!\HeaderTask: \@tasknum }
		\immediate\write\file{!\HeaderDate: \@date }
		\immediate\write\file{!\HeaderDetail: \detailAPDL }
	}{}
}

\let\oldsection\section
\renewcommand{\section}[1]{%
	\oldsection{#1}
	\iftoggle{exportcode}{\immediate\write\file{!!#1}}{}
}

\newcommand{\task}[1]{
	\hrule\medskip
	{
		\par\footnotesize{#1} 
	}
	\medskip\hrule \medskip
}

\newcommand{\unit}[1]{\,\mathrm{#1}\,}

\setlength{\parindent}{1ex}

\DeclareListParser{\codesep}{;}

\newcommand{\codelines}[1]
{
	\renewcommand*{\do}[1]
	{
		##1 \newline
		\iftoggle{exportcode}{\immediate\write\file{##1}}{}
	}
	\codesep{#1}
}	

\newcommand{\code}[1]
{
	\par\noindent\hspace{1cm}
	\begin{minipage}[t]{\dimexpr\linewidth - 1cm \relax}
		\ttfamily
		\codelines{#1}
		\vspace{-10pt}
	\end{minipage}
	\par
}

\newcommand{\comment}[2][2]
{
	\ifnum #1 > \detailPDF
	 % Not printing anything
	\else
	 #2 (#1)
	\fi
	\iftoggle{exportcode}{
		\ifnum #1 > \detailAPDL
		 % Not printing anything
		\else
		 \immediate\write\file{!#2 (#1)}
		\fi
	}{}
}

\newcommand{\ccode}[4][2]
{
	\par\noindent\hspace{1cm}
	\begin{minipage}[t]{#2}
		\ttfamily
		\codelines{#4}
		\vspace{-8pt}
	\end{minipage}%
	\ifnum #1 > \detailPDF
	 % Not printing anything
	\else
	 \begin{minipage}[t]{\dimexpr\linewidth - #2 - 1cm \relax}
	 	\par#3
	 \end{minipage}
	\fi
	\ifnum #1 > \detailAPDL
	% Not printing anything
	\else
	 \immediate\write\file{!#3 (#1)}
	\fi
	\par
}

\setlength{\parskip}{0pt}
\setlength{\fboxsep}{0pt}

\endinput