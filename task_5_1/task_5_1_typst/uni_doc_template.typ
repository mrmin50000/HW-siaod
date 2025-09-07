#let uni_doc(
  title-page: none,
  contents: true,
  language: "ru",
  number-pages: true,
  number-headings: true,
  hyphenate: true,
  doc
) = {
  set text(lang: language)

  let numbered_footer = none
  if number-pages {
    numbered_footer = auto
  }
  if title-page != none {
    set heading(outlined: false)
    title-page
    pagebreak()


    numbered_footer = context {
        let page_num = counter(page).get().first()
        if (page_num > 1) {
          align(center)[#page_num]
        }
    }
  }

  set page(
    numbering: "1",
    footer: numbered_footer,
    margin: (top: 1.5cm, left: 3cm, right: 1cm, bottom: 1.5cm),
  )

  set par(
    first-line-indent: (
      amount: 1.25cm, all: true
    ),
    spacing: 1.5cm,
    leading: 1.5em,
    justify: true
  )

  set text(font: "Times New Roman", size: 14pt)
  show heading: set text(size: 14pt)
  show figure.caption: set text(size: 12pt)

  set text(hyphenate: hyphenate)

  // Better listings format
  import "@preview/codly:1.3.0": *
  show: codly-init.with()
  codly(
    display-name: false,
  )

  let numbering = none
  if number-headings {
    numbering = "1."
  }
  set heading(numbering: numbering)

  if (contents) {
    outline()
    pagebreak()
  }

  doc
}