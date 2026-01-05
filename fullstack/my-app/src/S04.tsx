import { useState } from "react";

export default function S04() {
  const [username, setUsername] = useState("Alice");
  const [count, setCount] = useState(0);
  return (
    <div>
      <Header username={username} />
      <Counter count={count} />
    </div>
  );
}

function Header({ username }: { username: string }) {
  return <h1>Welcome, {username}!</h1>;
}

function Counter({ count }: { count: number }) {
  return <p>Current Count: {count}</p>;
}
