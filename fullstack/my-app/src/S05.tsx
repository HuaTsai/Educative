import { useState } from "react";

export default function S05() {
  const [username, setUsername] = useState("Alice");
  const [count, setCount] = useState(0);
  const callback = () => setCount((count) => count + 1);
  return (
    <div>
      <Header username={username} />
      <Counter count={count} onButtonClick={callback} />
    </div>
  );
}

function Header({ username }: { username: string }) {
  return <h1>Welcome, {username}!</h1>;
}

function Counter({
  count,
  onButtonClick,
}: {
  count: number;
  onButtonClick: () => void;
}) {
  return <button onClick={onButtonClick}>Current Count: {count}</button>;
}
