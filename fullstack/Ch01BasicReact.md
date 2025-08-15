# React with TypeScript Development Guide

## Project Setup

### Creating a React Project with Vite

```bash
# Create new project (recommended approach)
npx create-vite@latest react-examples
# or
npm create vite@latest react-examples

# Project configuration:
# Framework: React
# Variant: TypeScript + SWC

cd react-examples
npm install
npm run dev
```

> **Note**: Avoid global installation of `create-vite`. Using `npx` or `npm create` ensures you always use the latest version.

### npm Commands Summary

**Project Creation:**

- `npm create vite@latest` - Create Vite project (recommended)
- `npx create-vite@latest` - Alternative syntax
- Avoid global installation of create tools

**Package Installation:**

- `npm install package` - Production dependency
- `npm install -D package` - Development dependency (`--save-dev`)
- `npm list -g` - Check global packages

## JSX Fundamentals

JSX is a syntax extension for JavaScript that allows you to write HTML-like syntax within JavaScript.

### Key Features

- **HTML tag support**: Direct use of HTML tags
- **className attribute**: Use `className` instead of `class`
  ```jsx
  <div className="container">Content</div>
  ```
- **Component naming**: React components must start with uppercase letters
- **JavaScript expressions**: Use curly braces `{}` to embed JavaScript expressions
  ```jsx
  <img src={logoUrl} alt="Logo" />
  <p>Welcome back, {userName}!</p>
  ```

### Single Root Element Rule

JSX must return a single root element:

**❌ Incorrect:**

```jsx
return (
  <div>First div</div>
  <div>Second div</div>
);
```

**✅ Correct:**

```jsx
return (
  <div>
    <div>First div</div>
    <div>Second div</div>
  </div>
);
```

**✅ Using Fragment:**

```jsx
return (
  <>
    <div>First div</div>
    <div>Second div</div>
  </>
);
```

## TypeScript with React

### Import Patterns

```tsx
// Named imports (recommended)
import { Component, ReactNode } from "react";

// Namespace import
import React from "react";
// Usage: React.Component
```

### Component Props Type Definition

Define interfaces for component props to avoid TypeScript errors:

```tsx
interface ComponentProps {
  title: string;
  children: ReactNode; // For JSX content
  optional?: string; // Optional props
}

class MyComponent extends Component<ComponentProps> {
  render() {
    const { title, children } = this.props;
    return (
      <div>
        {title}: {children}
      </div>
    );
  }
}
```

### Children Property

React automatically passes content between component tags as the `children` prop:

```tsx
// These are equivalent:
<Component>Content here</Component>
<Component children="Content here" />

// ❌ Don't use both (causes conflict):
<Component children="prop">Tag content</Component>
```

## React Class Components

### Constructor and Class Properties

In TypeScript, class properties must be explicitly declared before use:

```tsx
interface ComponentProps {
  number: number;
}

class MyComponent extends Component<ComponentProps> {
  outputString: string; // TypeScript requires explicit declaration

  constructor(props: ComponentProps) {
    super(props); // Must call super() first
    this.outputString = "some value";
  }
}
```

**Super() in Constructor Rules:**

- `super(props)` **must be called first** in constructor
- Initializes the parent class (React.Component)
- Required before accessing `this.props` or `this`
- JavaScript/TypeScript inheritance rule

### Performance Best Practices

**❌ Bad - Computation runs on every render:**

```tsx
render() {
  const result = heavyCalculation(this.props.data); // Runs repeatedly
  return <div>{result}</div>;
}
```

**✅ Good - Computation runs once in constructor:**

```tsx
class MyComponent extends Component<Props> {
  result: string;

  constructor(props: Props) {
    super(props);
    this.result = heavyCalculation(props.data); // Runs once
  }

  render() {
    return <div>{this.result}</div>;
  }
}
```

## Array Handling and List Rendering

### TypeScript Array Declaration

```tsx
class MyComponent extends Component<Props> {
  outputArr: number[]; // Explicit array type declaration

  constructor(props: Props) {
    super(props);
    this.outputArr = []; // Initialize array
    this.outputArr.push(42); // Add elements
  }
}
```

### List Rendering with map()

React uses `map()` to convert arrays into JSX elements:

```tsx
render() {
  return (
    <ul>
      {
        this.outputArr.map((x, ind) => {
          return <li key={ind}>{x}</li>;
        })
      }
    </ul>
  );
}
```

**Map function parameters:**

- **First parameter (`x`)**: The array element value
- **Second parameter (`ind`)**: The array index (0, 1, 2, ...)

### React Keys

**Purpose of `key` Attribute:**

- **React-specific**: Not part of native HTML
- **Internal tracking**: Helps React identify which elements changed
- **Performance optimization**: Enables efficient re-rendering
- **Required for lists**: Must be unique within the list

**Key in Action:**

```tsx
// React JSX with keys
<li key={0}>17</li>
<li key={1}>52</li>

// Final HTML output (keys removed)
<li>17</li>
<li>52</li>

// Example: [1, 2, 3] → [1, 3, 4]
// With keys: React knows "2 removed, 4 added"
// Without keys: React might re-render all elements unnecessarily
```

## Styled-Components (CSS-in-JS)

### Installation

```bash
npm install styled-components
npm install -D @types/styled-components  # TypeScript support
```

**Package Dependencies:**

- `styled-components`: Production dependency (needed at runtime)
- `@types/styled-components`: Development dependency (TypeScript types only)

### Basic Styled Components

```tsx
import styled from "styled-components";

// Basic styled component
const GridDiv = styled.div`
  display: grid;
  grid-template-columns: 30% 70%;
  grid-gap: 10px;
  background-color: #eee;
  margin: 20px;
`;

// Extending styles
const EmphaticPar = styled.p`
  font-style: oblique;
`;

const VeryEmphaticPar = styled(EmphaticPar)`
  font-size: 150%;
`;
```

### Dynamic Props with TypeScript

```tsx
interface SpecialListItemProps {
  listStyleType?: string;
}

const SpecialListItem = styled.li<SpecialListItemProps>`
  font-weight: bold;
  font-size: 90%;
  padding: 2px;
  list-style-type: ${(props) => props.listStyleType || "square"};
`;

// Usage
<SpecialListItem>Default square</SpecialListItem>
<SpecialListItem listStyleType="circle">Circle bullet</SpecialListItem>
```

### CSS Grid Layout

```tsx
const GridDiv = styled.div`
  display: grid;
  grid-template-columns: 30% 70%;
  grid-gap: 10px;
`;

const LeftColumn = styled.div`
  grid-column: 1;
  margin: 0.8em;
`;

const RightColumn = styled.div`
  grid-column: 2;
  margin: 0.8em;
`;

// Usage
<GridDiv>
  <LeftColumn>Left content</LeftColumn>
  <RightColumn>Right content</RightColumn>
</GridDiv>
```

### Global Styles and Font Loading

Styled-components provides `createGlobalStyle` to inject global CSS rules into the entire application:

```tsx
import { createGlobalStyle } from "styled-components";

const GlobalStyle = createGlobalStyle`
  @font-face {
    font-family: 'Roboto';
    font-style: normal;
    font-weight: 100;
    font-display: swap;
    src: url(https://fonts.gstatic.com/s/roboto/v27/KFOkCnqEu92Fr1MmgVxIIzIXKMny.woff2) format('woff2');
    unicode-range: U+0000-00FF, U+0131, U+0152-0153, U+02BB-02BC, U+02C6, U+02DA, U+02DC, U+2000-206F, U+2074, U+20AC, U+2122, U+2191, U+2193, U+2212, U+2215, U+FEFF, U+FFFD;
  }
`;

export { GlobalStyle };
```

### Using GlobalStyle Component

GlobalStyle must be used as a component in your JSX:

```tsx
import { GlobalStyle } from "./fonts";

class App extends Component {
  render() {
    return (
      <>
        <GlobalStyle /> {/* Injects global styles */}
        <div>Your app content</div>
      </>
    );
  }
}
```

**Key Points:**

- `GlobalStyle` component injects CSS into the document `<head>`
- Must be placed in JSX to take effect
- Usually placed at the top level of your app
- Enables custom font loading without external CSS files

## Complete Example: Collatz Conjecture

This example demonstrates all the concepts covered in this guide, including styled-components, global fonts, TypeScript interfaces, and React class components.

### fonts.ts

```tsx
import { createGlobalStyle } from "styled-components";

const GlobalStyle = createGlobalStyle`
/* latin */
@font-face {
  font-family: 'Roboto';
  font-style: normal;
  font-weight: 100;
  font-display: swap;
  src: url(https://fonts.gstatic.com/s/roboto/v27/KFOkCnqEu92Fr1MmgVxIIzIXKMny.woff2) format('woff2');
  unicode-range: U+0000-00FF, U+0131, U+0152-0153, U+02BB-02BC, U+02C6, U+02DA, U+02DC, U+2000-206F, U+2074, U+20AC, U+2122, U+2191, U+2193, U+2212, U+2215, U+FEFF, U+FFFD;
}
`;

export { GlobalStyle };
```

### App.tsx

```tsx
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
  font-family: "Roboto", sans-serif;
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
              The <a href={wikilink}>Collatz Conjecture</a> states that for any
              positive integer, <i>x</i>, repeatedly applying <i>3x + 1</i> if
              odd and <i>x/2</i> if even will eventually lead to 1.
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
```
