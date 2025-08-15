import { Component } from "react";
import { GlobalStyle } from "./fonts";
import styled from "styled-components";

const GridDiv = styled.div`
  display: grid;
  grid-template-columns: 30% 70%;
  grid-template-rows: auto;
  grid-gap: 10px;
  background-color: #eee;
  color: black;
  margin: 20px;
`;

const LeftColumn = styled.div`
  grid-column: 1;
  margin: 0.8em;
`;

const RightColumn = styled.div`
  grid-column: 2;
  margin: 0.8em;
`;

const EmphaticPar = styled.p`
  font-style: oblique;
`;

const VeryEmphaticPar = styled(EmphaticPar)`
  font-family: 'Roboto', sans-serif;
  font-size: 150%;
`;

interface SpecialListItemProps {
  listStyleType?: string;
}

const SpecialListItem = styled.li<SpecialListItemProps>`
  font-weight: bold;
  font-size: 90%;
  padding: 2px;
  list-style-type: ${(props) => props.listStyleType || "square"};
`;

interface CollatzProps {
  number: number;
}

class Collatz extends Component<CollatzProps> {
  outputArr: number[];

  constructor(props: CollatzProps) {
    super(props);
    let x = this.props.number;
    this.outputArr = [x];
    while (typeof x == "number" && x !== 1) {
      if (x % 2 === 1) {
        x = 3 * x + 1;
      } else {
        x = x / 2;
      }
      this.outputArr.push(x);
    }
  }

  render() {
    const { number } = this.props;

    return (
      <div className="Collatz">
        <h2>Collatz Sequence for {number}</h2>
        <ul>
          {this.outputArr.map((x, ind) => {
            return <SpecialListItem key={ind}>{x}</SpecialListItem>;
          })}
        </ul>
      </div>
    );
  }
}

class App extends Component {
  render() {
    const number = 17;
    const wikilink = "https://en.wikipedia.org/wiki/Collatz_conjecture";
    return (
      <>
        <GlobalStyle />
        <GridDiv>
          <LeftColumn>
          <h1>Collatz Conjecture</h1>
          <p>
            The <a href={wikilink}>Collatz Conjecture </a>
            states that for any positive integer, <i>x</i>, repeatedly applying{" "}
            <i>3x + 1</i> if odd and <i>x/2</i> if even will eventually lead to
            1.
          </p>
          <VeryEmphaticPar>No one knows if this is true.</VeryEmphaticPar>
        </LeftColumn>
        <RightColumn>
          <Collatz number={number} />
        </RightColumn>
        </GridDiv>
      </>
    );
  }
}

export default App;
